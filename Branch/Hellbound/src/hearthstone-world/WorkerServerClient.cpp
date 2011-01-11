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

#include "StdAfx.h"

WSClient::WSClient(SOCKET fd) : Socket(fd, 1024576, 1024576)
{
	_remaining = 0;
	_cmd = 0;
}

WSClient::~WSClient()
{

}

void WSClient::OnRead()
{
	for(;;)
	{
		_size = (uint32)readBuffer.GetSize();
		if(!_cmd)
		{
			if(_size < 6)
				break;

			readBuffer.Read((uint8*)&_cmd, 2);
			readBuffer.Read((uint8*)&_remaining, 4);
		}

		if(_remaining && _size < _remaining)
			break;

		if(_cmd == IWSMSG_WOW_PACKET)
		{
			uint32 sid = 0;
			uint16 op = 0;
			uint32 sz = 0;
			readBuffer.Read(&sid, 4);
			readBuffer.Read(&op, 2);
			readBuffer.Read(&sz, 4);

			Log.Debug("WorkerServer", "Recieved Opcode %u", op);
			WorldSession* session = sWorld.FindSession(sid);
			if(session != NULL)
			{
				Log.Debug("WorkerServer", "Found session for Session ID %u", sid);
				WorldPacket * pck = new WorldPacket(op, sz);
				if (sz > 0)
				{
					pck->resize(sz);
					readBuffer.Read((void*)pck->contents(), sz);
				}

				session->QueuePacket(pck);
			}
			_remaining = _size = _cmd = 0;
			continue;
		}

		WorldPacket * pck = new WorldPacket(_cmd, _remaining);
		Log.Notice("PacketHandler", "Recieved Packet %u with size %u\n", _cmd, _remaining);
		if(_remaining > 0)
		{
			pck->resize(_remaining);
			readBuffer.Read((uint8*)pck->contents(), _remaining);
		}
		_remaining = _size = _cmd = 0;

		/* we could handle auth here */
		sClusterInterface.QueuePacket(pck);
	}
}

void WSClient::OnConnect()
{
	sClusterInterface.SetSocket(this);
}

void WSClient::OnDisconnect()
{
	sClusterInterface.ConnectionDropped();
	sSocketGarbageCollector.QueueSocket(this);
}

void WSClient::SendPacket(WorldPacket * data)
{
	bool rv;
	uint32 size = (uint32)data->size();
	uint16 opcode = data->GetOpcode();
	if(!IsConnected())
		return;

	BurstBegin();

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&opcode, 2);
	rv = BurstSend((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = BurstSend((const uint8*)data->contents(), size);

	if(rv) BurstPush();
	BurstEnd();
}
