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

WSSocket::WSSocket(SOCKET fd, const sockaddr_in * peer) : TcpSocket(fd, 100000, 100000, false, peer)
{
	_authenticated = false;
	_remaining = 0;
	_cmd = 0;
	_ws = NULL;
}

WSSocket::~WSSocket()
{

}

void WSSocket::HandleAuthRequest(WorldPacket & pck)
{
	string ver;
	uint32 build;
	uint8 key[20];
	uint32 result = 1;
	pck.read(key, 20);

	// check if we have the correct password
	if(memcmp(key, sClusterMgr.key, 20)) // todo
		result = 0;
	pck >> build;
	pck >> ver;

	Log.Notice("WSSocket", "Auth reply, server is %s build %u, result %s.", ver.c_str(), build, result ? "OK" : "FAIL");

	printf("Key: ");
	for(int i = 0; i < 20; ++i)
		printf("%.2X", key[i]);
	printf("\n");

	// accept it
	WorldPacket data(SMSGR_AUTH_RESULT, 4);
	data << uint32(result);
	SendPacket(&data);

	_authenticated = result ? true : false;
}

void WSSocket::OnRecvData()
{
	for(;;)
	{
		if(!_cmd)
		{
			if(GetReadBuffer()->GetSize() < 6)
				break;

			Read(&_cmd, 2);
			Read(&_remaining, 4);
			//_remaining = ntohl(_remaining);
		}

		if(_remaining && GetReadBuffer()->GetSize() < _remaining)
			return;

		if(_cmd == CMSGR_WOW_PACKET)
		{
			/* optimized version for packet passing, to reduce latency! ;) */
			uint32 sid;
			uint16 op;
			uint32 sz;
			Read(&sid, 4);
			Read(&op, 2);
			Read(&sz, 4);
			WorldPacket * pck = new WorldPacket(op, sz);
			if(sz)
			{
				pck->resize(sz);
				Read((uint8*)pck->contents(), sz);
			}

			Session * session = sClientMgr.GetSession(sid);
			if(session != NULL)
				session->SendPacket(pck);

			_cmd = 0;
			continue;
		}

		WorldPacket * pck = new WorldPacket(_cmd, _remaining);
		_cmd = 0;
		if(_remaining)
		{
			pck->resize(_remaining);
			Read((uint8*)pck->contents(), _remaining);
		}

		if(_authenticated)
		{
			// push to queue
			if(!_ws)
			{
				if(pck->GetOpcode() == CMSGR_REGISTER_WORKER) // handle register worker
					HandleRegisterWorker(*pck);

				/* I deliberately don't delete pck here for a reason :P */
			}
			else
			{
				_ws->QueuePacket(pck);
			}
		}
		else
		{
			if(pck->GetOpcode() != CMSGR_AUTH_REPLY)
				Disconnect();
			else
				HandleAuthRequest(*pck);
			delete pck;
		}
	}
}

void WSSocket::HandleRegisterWorker(WorldPacket & pck)
{
	uint32 build;
	pck >> build;

	// TODO: Check the build of the server
	WServer * new_server = sClusterMgr.CreateWorkerServer(this);
	if(new_server == 0)
	{
		WorldPacket data(SMSGR_REGISTER_RESULT, 10);
		data << uint32(0);
		SendPacket(&data);
		delete &pck;
		return;
	}

	/* because we don't have any locks in the managers, this has to execute
	   in the other thread. this is why I haven't deleted the packet yet
	 */
    _ws = new_server;
	pck.rpos(0);
	_ws->QueuePacket(&pck);
}

void WSSocket::SendPacket(WorldPacket * pck)
{
	bool rv;
	size_t size = pck->size();
	uint16 opcode = pck->GetOpcode();
	if(!IsConnected())
		return;

	LockWriteBuffer();

	// Pass the header to our send buffer
	rv = WriteButHold((const uint8*)&opcode, 2);
	rv = WriteButHold((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = Write((const uint8*)pck->contents(), uint32(size));
	else if(rv)
		rv = ForceSend();

	UnlockWriteBuffer();
}

void WSSocket::SendWoWPacket(Session * from, WorldPacket * pck)
{
	bool rv;
	size_t size1 = pck->size();
	uint16 opcode1 = pck->GetOpcode();
	size_t size2 = size1 + 10;
	uint32 opcode2 = SMSGR_WOW_PACKET;
	uint32 id = from->GetSessionId();
	if(!IsConnected())
		return;

	LockWriteBuffer();

	// Pass the header to our send buffer
	WriteButHold((const uint8*)&opcode2, 2);
	WriteButHold((const uint8*)&size2, 4);
	WriteButHold((const uint8*)&id, 4);
	WriteButHold((const uint8*)&opcode1, 2);
	rv = WriteButHold((const uint8*)&size1, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size1 > 0 && rv)
		rv = Write(pck->contents(), uint32(size1));
	else if(rv)
		rv = ForceSend();

	UnlockWriteBuffer();
}

void WSSocket::OnConnect()
{
	WorldPacket data(SMSGR_AUTH_REQUEST, 4);
	data << uint32(BUILD_REVISION);
	SendPacket(&data);
}
