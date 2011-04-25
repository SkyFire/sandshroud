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

WServerHandler MasterServer::PHandlers[IMSG_NUM_TYPES];

void MasterServer::InitHandlers()
{
	memset(PHandlers, 0, sizeof(void*) * IMSG_NUM_TYPES);
	PHandlers[IWSMSG_SERVER_PING]		= &MasterServer::HandleServerPing;
	PHandlers[IMSMSG_SERVER_PONG]		= NULL;
	PHandlers[IMSMSG_WOW_PACKET]		= &MasterServer::HandleWoWPacket;
	PHandlers[IMSG_PLAYER_TRANSFER]		= &MasterServer::HandlePlayerLoginResponse;
}

MasterServer::MasterServer(MasterServerSocket * s) : m_socket(s)
{
	last_ping = 0;
	removed = false;
}

MasterServer::~MasterServer()
{
	m_socket = NULL;
	m_recvQueue.Clear();
}

void MasterServer::HandleServerPing(WorldPacket & recvData)
{
	Log.Notice("MasterSocket", "Ping");
	SendPong();
}

void MasterServer::SendPong()
{
	uint32 t = uint32(UNIXTIME);
	m_socket->SendPacket(IMSMSG_SERVER_PONG, 4, &t);
	last_ping = t;
}

void MasterServer::HandlePlayerLoginResponse(WorldPacket & pck)
{
	uint8 success;
	uint32 guid, sessionid;
	pck >> guid >> sessionid >> success;
	Session* m_session = sClientMgr.GetSession(sessionid);
	if(m_session)
	{
		if(!success)
		{
			WorldPacket data(SMSG_CHARACTER_LOGIN_FAILED, 1);
			data << uint8(CHAR_LOGIN_NO_WORLD);
			m_session->SendPacket(&data);
		}
		else
		{
			// Transfer complete
		}
	}
}

void MasterServer::HandleWoWPacket(WorldPacket & pck)
{
	uint32 sessionid, size;
	uint16 opcode;

	/* get session */
	pck >> sessionid >> opcode >> size;
	DEBUG_LOG("HandleWoWPacket", "Recieved packet %u to forward.", opcode);

	Session * session = sClientMgr.GetSession(sessionid);
	if(!session)
		return;

	/* write it to that session's output buffer */
	ClientSocket * s = session->GetSocket();
	if(s)
		s->OutPacket(opcode, size, size ? ((const void*)(pck.contents() + 10)) : 0);
}

void MasterServer::Update()
{
	UpdateMutex.Acquire();
	uint32 t = (uint32)UNIXTIME;
	if(!removed && ((last_ping > 0) && ((t - last_ping) > 40)))
	{
		// ping timeout
		removed = true;
		Log.Notice("MasterServer", "Ping timeout. Dropping Master Server.");
		m_socket->Disconnect();
		m_recvQueue.Clear();
		m_socket = NULL;
		UpdateMutex.Release();

		sClusterMgr.OnMasterServerDisconnect();
		return;
	}

	uint16 opcode;
	WorldPacket * pck;
	while((pck = m_recvQueue.Pop()))
	{
		opcode = pck->GetOpcode();
		if(opcode < IMSG_NUM_TYPES && MasterServer::PHandlers[opcode] != 0)
			(this->*MasterServer::PHandlers[opcode])(*pck);
		else
			Log.Error("MasterServer", "Unhandled packet %u\n", opcode);
	}
	UpdateMutex.Release();
}
