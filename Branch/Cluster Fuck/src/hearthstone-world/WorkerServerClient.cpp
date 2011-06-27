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

WSClient::WSClient(SOCKET fd, const sockaddr_in * peer) : TcpSocket(fd, 1024576, 1024576, false, peer)
{
	_remaining = 0;
	_cmd = 0;
}

WSClient::~WSClient()
{

}

void WSClient::OnRecvData()
{
	for(;;)
	{
		if(!_cmd)
		{
			if(GetReadBuffer()->GetSize() < 6)
				break;

			Read((uint8*)&_cmd, 2);
			Read((uint8*)&_remaining, 4);
		}

		if(_remaining && GetReadBuffer()->GetSize() < _remaining)
			return;

		if(_cmd == SMSGR_WOW_PACKET)
		{
			uint32 sid = 0;
			uint16 op = 0;
			uint32 sz = 0;
			Read(&sid, 4);
			Read(&op, 2);
			Read(&sz, 4);

			WorldSession * session = sClusterInterface.GetSession(sid);
			if(session != NULL)
			{
				WorldPacket * pck = new WorldPacket(op, sz);
				if (sz > 0)
				{
					pck->resize(sz);
					Read((void*)pck->contents(), sz);
				}

				if(session)
					session->QueuePacket(pck);
				else
					delete pck;
			}
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

		/* we could handle auth here */
		switch(_cmd)
		{
		case SMSGR_AUTH_REQUEST:
			sClusterInterface.HandleAuthRequest(*pck);
			delete pck;
			break;
		default:
			sClusterInterface.QueuePacket(pck);
		}
	}
}

void WSClient::OnConnect()
{
	sClusterInterface.SetSocket(this);
}

void WSClient::OnDisconnect()
{
	sClusterInterface.ConnectionDropped();
	sSocketDeleter.Add(this);
}

void WSClient::SendPacket(WorldPacket * data)
{
	bool rv;
	uint32 size = (uint32)data->size();
	uint16 opcode = data->GetOpcode();
	if(!IsConnected())
		return;

	LockWriteBuffer();

	// Pass the header to our send buffer
	rv = WriteButHold((const uint8*)&opcode, 2);
	rv = WriteButHold((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = Write((const uint8*)data->contents(), size);
	else if(rv)
		rv = ForceSend();

	UnlockWriteBuffer();
}
