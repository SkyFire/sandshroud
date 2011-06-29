/*
 * Sandshroud Hearthstone
 * Copyright (C) 2010 - 2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "RStdAfx.h"

#pragma pack(push, 1)
struct ClientPktHeader
{
	uint16 size;
	uint32 cmd;
};

struct ServerPktHeader
{
	uint16 size;
	uint16 cmd;
};
#pragma pack(pop)

WorldSocket::WorldSocket(SOCKET fd, const sockaddr_in * peer) : TcpSocket(fd, WORLDSOCKET_SENDBUF_SIZE, WORLDSOCKET_RECVBUF_SIZE, false, peer)
{
	mSize = mOpcode = mRemaining = 0;
	_latency = 0;
	mSeed = rand() % 0xFFFFFFF0 + 10;
	pAuthenticationPacket = NULL;
	mRequestID = 0;
	Authed = false;
	mQueued = false;
	m_session = NULL;
	m_nagleEanbled = false;
	m_fullAccountName = NULL;
}

WorldSocket::~WorldSocket()
{
	WorldPacket * pck;
	queueLock.Acquire();
	while((pck = _queue.Pop()))
		delete pck;
	queueLock.Release();
}

void WorldSocket::OnDisconnect()
{
	if(m_session)
	{
		m_session->m_socket = NULL;
		m_session = NULL;
	}

	if(mRequestID != 0)
	{
		sLogonCommHandler.UnauthedSocketClose(mRequestID);
		mRequestID = 0;
	}

	// clear buffer
	queueLock.Acquire();
	WorldPacket *pck;
	while((pck = _queue.Pop()))
		delete pck;
	queueLock.Release();
}

void WorldSocket::OutPacket(uint16 opcode, size_t len, const void* data)
{
	OUTPACKET_RESULT res;
	if( (len + 10) > WORLDSOCKET_SENDBUF_SIZE )
	{
		printf("WARNING: Tried to send a packet of %u bytes (which is too large) to a socket. Opcode was: %u (0x%03X)\n", uint(len), uint(opcode), uint(opcode));
		return;
	}

	res = _OutPacket(opcode, len, data);
	Log.Notice("WorldSocket", "Outpacket result %s", (res == OUTPACKET_RESULT_SUCCESS ? "Success" : (res == OUTPACKET_RESULT_NO_ROOM_IN_BUFFER ? "No Room" : (res == OUTPACKET_RESULT_NOT_CONNECTED ? "Not Connected" : "Socket Error"))));
	if(res == OUTPACKET_RESULT_SUCCESS)
		return;

	if(res == OUTPACKET_RESULT_NO_ROOM_IN_BUFFER)
	{
		/* queue the packet */
		queueLock.Acquire();
		WorldPacket * pck = new WorldPacket(opcode, len);
		pck->SetOpcode(opcode);
		if(len)
			pck->append((const uint8*)data, len);
		_queue.Push(pck);
		queueLock.Release();
	}
}

void WorldSocket::UpdateQueuedPackets()
{
	queueLock.Acquire();
	if(!_queue.HasItems())
	{
		queueLock.Release();
		return;
	}

	WorldPacket * pck;
	while((pck = _queue.front()))
	{
		/* try to push out as many as you can */
		switch(_OutPacket(pck->GetOpcode(), pck->size(), pck->size() ? pck->contents() : NULL))
		{
		case OUTPACKET_RESULT_SUCCESS:
			{
				delete pck;
				_queue.pop_front();
			}break;

		case OUTPACKET_RESULT_NO_ROOM_IN_BUFFER:
			{
				/* still connected */
				queueLock.Release();
				return;
			}break;

		default:
			{
				/* kill everything in the buffer */
				while((pck = _queue.Pop()))
					delete pck;
				queueLock.Release();
				return;
			}break;
		}
	}
	queueLock.Release();
}

OUTPACKET_RESULT WorldSocket::_OutPacket(uint16 opcode, size_t len, const void* data)
{
	bool rv;
	if(!IsConnected())
		return OUTPACKET_RESULT_NOT_CONNECTED;

	if( GetWriteBuffer()->GetSpace() < (len+4) )
		return OUTPACKET_RESULT_NO_ROOM_IN_BUFFER;

	LockWriteBuffer();
	// Encrypt the packet
	// First, create the header.
	ServerPktHeader Header;
	Header.cmd = opcode;
	Header.size = ntohs((uint16)len + 2);
	_crypt.EncryptSend((uint8*)&Header, sizeof (ServerPktHeader));

	// Pass the header to our send buffer
	rv = WriteButHold((const uint8*)&Header, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(len > 0 && rv)
		rv = Write((const uint8*)data, (uint32)len);
	else if(rv)
		rv = ForceSend();

	UnlockWriteBuffer();
	return rv ? OUTPACKET_RESULT_SUCCESS : OUTPACKET_RESULT_SOCKET_ERROR;
}

void WorldSocket::OnConnect()
{
	_latency = getMSTime();
	WorldPacket data (SMSG_AUTH_CHALLENGE, 25);
	data << uint32(1);			// Unk
	data << mSeed;
	data << uint32(0xF3539DA3);	// Generated Random.
	data << uint32(0x6E8547B9);	// 3.2.2
	data << uint32(0x9A6AA2F8);	// 3.2.2
	data << uint32(0xA4F170F4);	// 3.2.2
	SendPacket(&data);
}

void WorldSocket::_HandleAuthSession(WorldPacket* recvPacket)
{
	std::string account;
	uint32 unk;
	uint64 unk3; // 3.2.2 Unk
	_latency = getMSTime() - _latency;

	try
	{
		*recvPacket >> mClientBuild;
		*recvPacket >> unk;
		*recvPacket >> account;
		*recvPacket >> unk;
		*recvPacket >> mClientSeed;
		// 3.2.2
		*recvPacket >> unk3;
		// 3.3.5
		*recvPacket >> unk;
		*recvPacket >> unk;
		*recvPacket >> unk;
	}
	catch(ByteBuffer::error &)
	{
		OUT_DEBUG("Incomplete copy of AUTH_SESSION Received.");
		return;
	}

	if(mClientBuild != CL_BUILD_SUPPORT)
	{
		OUT_DEBUG("Mismatched Client Rejected.");
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x14");
		return;
	}

	// Send out a request for this account.
	mRequestID = sLogonCommHandler.ClientConnected(account, this);
	if(mRequestID == 0xFFFFFFFF)
	{
		OUT_DEBUG("Request denied.");
		Disconnect();
		return;
	}

	m_fullAccountName = new string(account);

	// Set the authentication packet 
	pAuthenticationPacket = recvPacket;
}

void WorldSocket::InformationRetreiveCallback(WorldPacket & recvData, uint32 requestid)
{
	if(requestid != mRequestID)
		return;

	uint32 error;
	recvData >> error;
	if(error != 0 || pAuthenticationPacket == NULL)
	{
		// something happened wrong @ the logon server
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
		return;
	}

	// Extract account information from the packet.
	string AccountName;
	const string * ForcedPermissions;
	uint32 AccountID;
	string GMFlags;
	uint8 AccountFlags;
	string lang = "enUS";

	recvData >> AccountID >> AccountName >> GMFlags >> AccountFlags;
	ForcedPermissions = sLogonCommHandler.GetForcedPermissions(AccountName);
	if( ForcedPermissions != NULL )
		GMFlags.assign(ForcedPermissions->c_str());

	DEBUG_LOG( "WorldSocket","Received information packet from logon: `%s` ID %u (request %u)", AccountName.c_str(), AccountID, mRequestID);
//	sLog.outColor(TNORMAL, "\n");

	mRequestID = 0;
	//Pull the session key.

	BigNumber BNK;
	recvData.read(K, 40);
	_crypt.Init(K);
	BNK.SetBinary(K, 40);

	//checking if player is already connected
	//disconnect current player and login this one(blizzlike)

	if(recvData.rpos() != recvData.wpos())
		recvData.read((uint8*)lang.data(), 4);

	Session* session =/* sClientMgr.GetSessionByAccountId(AccountID);
	if( session != NULL )
	{
		if(session->_player != NULL && session->_player->GetMapMgr() == NULL)
		{
			DEBUG_LOG("WorldSocket","_player found without m_mapmgr during logon, trying to remove him [player %s, map %d, instance %d].", session->_player->GetName(), session->_player->GetMapId(), session->_player->GetInstanceID() );
			if(objmgr.GetPlayer(session->_player->GetLowGUID()))
				objmgr.RemovePlayer(session->_player);
			session->LogoutPlayer(false);
		}
		// AUTH_FAILED = 0x0D
		session->Disconnect();

		// clear the logout timer so he times out straight away
//		session->SetLogoutTimer(1);

		// we must send authentication failed here.
		// the stupid newb can relog his client.
		// otherwise accounts dupe up and disasters happen.
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x15");
		return;
	}
	session =*/ NULL;

	Sha1Hash sha;

	uint8 digest[20];
	pAuthenticationPacket->read(digest, 20);

	uint32 t = 0;
	if( m_fullAccountName == NULL )				// should never happen !
		sha.UpdateData(AccountName);
	else
	{
		sha.UpdateData(*m_fullAccountName);

		// this is unused now. we may as well free up the memory.
		delete m_fullAccountName;
		m_fullAccountName = NULL;
	}

	sha.UpdateData((uint8 *)&t, 4);
	sha.UpdateData((uint8 *)&mClientSeed, 4);
	sha.UpdateData((uint8 *)&mSeed, 4);
	sha.UpdateBigNumbers(&BNK, NULL);
	sha.Finalize();

	if (memcmp(sha.GetDigest(), digest, 20))
	{
		// AUTH_UNKNOWN_ACCOUNT = 21
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x15");
		return;
	}

	session = sClientMgr.CreateSession(AccountID);
	m_session = session;
	ASSERT(m_session);
	session->m_socket = this;

	// Set session properties
	session->m_accountFlags = AccountFlags;
	session->m_GMPermissions = GMFlags;
	session->m_accountId = AccountID;
	session->m_latency = _latency;
	session->m_accountName = AccountName;
	if(recvData.rpos() != recvData.wpos())
		recvData >> session->m_muted;

	for(uint32 i = 0; i < 8; i++)
		session->SetAccountData(i, NULL, true, 0);

	DEBUG_LOG("Auth", "%s from %s:%u [%ums]", AccountName.c_str(), GetIP(), GetPort(), _latency);
	Authenticate();
}

void WorldSocket::Authenticate()
{
	delete pAuthenticationPacket;
	pAuthenticationPacket = NULL;

	if(m_session->m_accountFlags & 16)
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x02");
	else if(m_session->m_accountFlags & 8)
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x01");
	else
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x00");
}

void WorldSocket::_HandlePing(WorldPacket* recvPacket)
{
	uint32 ping;
	if(recvPacket->size() < 4)
	{
		sLog.outString("Socket closed due to incomplete ping packet.");
		Disconnect();
		return;
	}

	*recvPacket >> ping;
	*recvPacket >> _latency;
	OutPacket(SMSG_PONG, 4, &ping);

#ifdef WIN32
	// Dynamically change nagle buffering status based on latency.
	if(_latency >= 250)
	{
		if(!m_nagleEanbled)
		{
			u_long arg = 0;
			setsockopt(GetFd(), 0x6, 0x1, (const char*)&arg, sizeof(arg));
			m_nagleEanbled = true;
		}
	}
	else
	{
		if(m_nagleEanbled)
		{
			u_long arg = 1;
			setsockopt(GetFd(), 0x6, 0x1, (const char*)&arg, sizeof(arg));
			m_nagleEanbled = false;
		}
	}
#endif
}

void WorldSocket::OnRecvData()
{
	for(;;)
	{
		// Check for the header if we don't have any bytes to wait for.
		if(mRemaining == 0)
		{
			if(GetReadBuffer()->GetSize() < 6)
			{
				// No header in the packet, let's wait.
				return;
			}

			// Copy from packet buffer into header local var
			ClientPktHeader Header;
			Read(&Header, 6);

			// Decrypt the header
			_crypt.DecryptRecv((uint8*)&Header, sizeof (ClientPktHeader));
			mRemaining = mSize = ntohs(Header.size) - 4;
			mOpcode = Header.cmd;
		}

		if(mRemaining > 0)
		{
			if( GetReadBuffer()->GetSize() < mRemaining )
			{
				// We have a fragmented packet. Wait for the complete one before proceeding.
				return;
			}
		}

		WorldPacket *Packet = new WorldPacket(mOpcode, mSize);
		if(mRemaining > 0)
		{
			Packet->resize(mRemaining);
			Read((uint8*)Packet->contents(), mRemaining);
		}
		mRemaining = mSize = mOpcode = 0;

		// Check for packets that we handle
		DEBUG_LOG("WorldSocket", "Received Opcode: %u", Packet->GetOpcode());
		switch(Packet->GetOpcode())
		{
		case CMSG_PING:
			{
				if(!m_session->m_currentPlayer)
				{
					_HandlePing(Packet);
					delete Packet;
				}
				else
					m_session->QueuePacket(Packet);
			}break;
		case CMSG_AUTH_SESSION:
			{
				_HandleAuthSession(Packet);
			}break;
		default:
			{
				if(m_session)
					m_session->QueuePacket(Packet);
				else
					delete Packet;
			}break;
		}
	}
}
