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

MasterServerSocket::MasterServerSocket(SOCKET fd) : Socket(fd, 100000, 100000)
{
	_authenticated = false;
	_remaining = 0;
	_cmd = 0;
	Master = NULL;
}

MasterServerSocket::~MasterServerSocket()
{

}

void MasterServerSocket::HandleAuthRequest(WorldPacket & pck)
{
	uint8 key[20];
	uint32 build;
	string ver;
	pck.read(key, 20);
	pck >> build;
	pck >> ver;

	Log.Notice("MasterSocket", "Auth reply, server is %s build %u", ver.c_str(), build);

	// accept it
	build = 1;
	SendPacket(IWSMSG_AUTH_RESULT, 4, &build);
	_authenticated = true;
}

void MasterServerSocket::OnRead()
{
	uint32 size = 0;
	for(;;)
	{
		size = (uint32)readBuffer.GetSize();
		if(!_cmd)
		{
			if(size < 6)
				break;

			readBuffer.Read(&_cmd, 2);
			readBuffer.Read(&_remaining, 4);
		}

		if(_remaining && size < _remaining)
			break;

		WorldPacket * pck;
		if(_cmd == IMSMSG_WOW_PACKET)
		{
			/* optimized version for packet passing, to reduce latency! ;) */
			uint32 sid;
			uint16 op;
			uint32 sz;
			readBuffer.Read(&sid, 4);
			readBuffer.Read(&op, 2);
			readBuffer.Read(&sz, 4);
			pck = new WorldPacket(op, sz);
			pck->resize(sz);
			readBuffer.Read((uint8*)pck->contents(), sz);

			Session * session = sClientMgr.GetSession(sid);
			if(session != NULL)
				session->SendPacket(pck);

			_cmd = 0;
			continue;
		}

		pck = new WorldPacket(_cmd, _remaining);
		if(_remaining > 0)
		{
			// Copy from packet buffer into our actual buffer.
			//Read(mRemaining, (uint8*)Packet->contents());
			pck->resize(_remaining);
			readBuffer.Read((uint8*)pck->contents(), _remaining);
		}
		_remaining = size = _cmd = 0;

		if(_authenticated)
		{
			// push to queue
			if(!Master)
			{
				if(pck->GetOpcode() == IMSMSG_REGISTER_MASTER) // handle register worker
				{
					HandleRegisterMaster(*pck);
					delete pck;
					continue;
				}
			}
			else if(pck->GetOpcode() != IMSMSG_REGISTER_MASTER)
			{
				Master->QueuePacket(pck);
				continue;
			}
			delete pck;
		}
		else
		{
			if(pck->GetOpcode() != IWSMSG_SERVER_PING)
			{
				if(pck->GetOpcode() != IMSMSG_AUTH_REPLY)
					Disconnect();
				else
					HandleAuthRequest(*pck);
			}
			delete pck;
		}
	}
}

void MasterServerSocket::HandleRegisterMaster(WorldPacket & pck)
{
	uint32 build;
	pck >> build;

	// TODO: Check the build of the server
	Log.Notice("MasterSocket", "Registering Master Server, build %u", build);
	MasterServer * new_server = sClusterMgr.CreateMasterServer(this);
	if(new_server == 0)
	{
		WorldPacket data(IWSMSG_REGISTER_RESULT, 10);
		data << uint32(0);
		SendPacket(&data);
		return;
	}

	Log.Notice("MasterSocket", "Master Server Registered");
	/* because we don't have any locks in the managers, this has to execute
	   in the other thread. this is why I haven't deleted the packet yet
	 */
	WorldPacket data(IWSMSG_REGISTER_RESULT, 10);
	data << uint32(1);
	SendPacket(&data);
    Master = new_server;
	Master->SendPong();
}

void MasterServerSocket::SendPacket(WorldPacket * pck)
{
	bool rv;
	size_t size = pck->size();
	uint16 opcode = pck->GetOpcode();
	if(!IsConnected())
		return;

	BurstBegin();

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&opcode, 2);
	rv = BurstSend((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = BurstSend((const uint8*)pck->contents(), uint32(size));

	if(rv)
		BurstPush();
	BurstEnd();
}

void MasterServerSocket::SendPacket(uint16 opcode, uint32 size, const void* data)
{
	bool rv;
	if(!IsConnected())
		return;

	BurstBegin();

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&opcode, 2);
	rv = BurstSend((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = BurstSend((const uint8*)data, uint32(size));

	if(rv)
		BurstPush();
	BurstEnd();
}

void MasterServerSocket::SendWoWPacket(Session * from, WorldPacket * pck)
{
	bool rv;
	uint32 CompleteOpcodeSize = uint32(pck->size());
	uint16 RSOpcode = IWSMSG_WOW_PACKET;
	uint32 RSOpcodeSize = CompleteOpcodeSize + 10;
	uint32 SessionId = from->GetSessionId();
	uint16 WoWOpcode = pck->GetOpcode();
	uint32 WoWOpcodeSize = CompleteOpcodeSize;
	if(!IsConnected())
		return;

	BurstBegin();

	Log.Notice("MasterSocket", "Sending WoW Packet %u to Master Server", WoWOpcode);
	// Pass the header to our send buffer
	BurstSend((const uint8*)&RSOpcode, 2);
	BurstSend((const uint8*)&RSOpcodeSize, 4);
	BurstSend((const uint8*)&SessionId, 4);
	BurstSend((const uint8*)&WoWOpcode, 2);
	rv = BurstSend((const uint8*)&WoWOpcodeSize, 4);	

	// Pass the rest of the packet to our send buffer (if there is any)
	if(WoWOpcodeSize > 0 && rv)
		rv = BurstSend(pck->contents(), uint32(WoWOpcodeSize));

	if(rv)
		BurstPush();
	BurstEnd();
}

void MasterServerSocket::OnConnect()
{
	Log.Notice("MasterSocket", "Sending Auth Request");
	WorldPacket data(IWSMSG_AUTH_REQUEST, 4);
	data << uint32(BUILD_REVISION);
	SendPacket(&data);
}
