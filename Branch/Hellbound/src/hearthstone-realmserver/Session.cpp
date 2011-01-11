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

OpcodeHandler Handlers[NUM_MSG_TYPES];

void Session::InitHandlers()
{
	for(uint32 i = 0; i < NUM_MSG_TYPES; i++)
	{
		Handlers[i].status = 1;
		Handlers[i].handler = NULL;
	}

	Handlers[CMSG_PLAYER_LOGIN].status						= 0;
	Handlers[CMSG_PLAYER_LOGIN].handler						= &Session::HandlePlayerLogin;

	Handlers[CMSG_LOGOUT_REQUEST].status					= 0;
	Handlers[CMSG_LOGOUT_REQUEST].handler					= &Session::HandlePlayerLogoutRequest;

	Handlers[CMSG_CHAR_ENUM].status							= 0;
	Handlers[CMSG_CHAR_ENUM].handler						= &Session::HandleCharacterEnum;

	Handlers[CMSG_CHAR_CREATE].status						= 0;
	Handlers[CMSG_CHAR_CREATE].handler						= &Session::HandleCharacterCreate;

	Handlers[CMSG_CHAR_DELETE].status						= 0;
	Handlers[CMSG_CHAR_DELETE].handler						= &Session::HandleCharacterDelete;

	Handlers[CMSG_CHAR_RENAME].status						= 0;
	Handlers[CMSG_CHAR_RENAME].handler						= &Session::HandleCharacterRename;

	Handlers[CMSG_CHAR_CUSTOMIZE].status					= 0;
	Handlers[CMSG_CHAR_CUSTOMIZE].handler					= &Session::HandleCharacterCustomize;

	Handlers[CMSG_REALM_SPLIT].status						= 0;
	Handlers[CMSG_REALM_SPLIT].handler						= &Session::HandleRealmSplitQuery;

	Handlers[CMSG_QUERY_TIME].status						= 0;
	Handlers[CMSG_QUERY_TIME].handler						= &Session::HandleQueryTimeOpcode;

	Handlers[CMSG_UPDATE_ACCOUNT_DATA].status				= 2;
	Handlers[CMSG_UPDATE_ACCOUNT_DATA].handler				= &Session::HandleUpdateAccountData;

	Handlers[CMSG_REQUEST_ACCOUNT_DATA].status				= 2;
	Handlers[CMSG_REQUEST_ACCOUNT_DATA].handler				= &Session::HandleRequestAccountData;

	Handlers[CMSG_READY_FOR_ACCOUNT_DATA_TIMES].status		= 2;
	Handlers[CMSG_READY_FOR_ACCOUNT_DATA_TIMES].handler		= &Session::HandleReadyForAccountDataTimes;

	Handlers[CMSG_VOICE_SESSION_ENABLE].status				= 0;
	Handlers[CMSG_VOICE_SESSION_ENABLE].handler				= &Session::HandleEnableMicrophoneOpcode;

	Handlers[CMSG_SET_ACTIVE_VOICE_CHANNEL].status			= 0;
	Handlers[CMSG_SET_ACTIVE_VOICE_CHANNEL].handler			= &Session::HandleVoiceChatQueryOpcode;
}

Session::Session(uint32 id) : m_sessionId(id)
{
	m_socket = NULL;
	m_accountId = 0;
	m_ClientBuild = 0;
	m_latency = 0;
	m_accountFlags = 0;
	m_highestLevel = 1;
	m_language = 0;
	m_muted = 0;
	deleted = false;
	OnMasterServer = false;
	m_hasDeathKnight = false;
	m_loadedPlayerData = false;

	for(uint32 x = 0; x < 8; x++)
	{
		sAccountData[x].sz = NULL;
		sAccountData[x].data = NULL;
	}
}

Session::~Session()
{

}

void Session::Update()
{
	uint16 opcode;
	WorldPacket * pck;
	MasterServer* MasterControl = sClusterMgr.GetMasterServer();
	while((pck = m_readQueue.Pop()))
	{
		opcode = pck->GetOpcode();

		if(MasterControl && Handlers[opcode].status == 1)
			MasterControl->SendWoWPacket(this, pck);
		else if(MasterControl && Handlers[opcode].status == 2 && OnMasterServer)
			MasterControl->SendWoWPacket(this, pck);
		else if(Handlers[opcode].handler != 0) // can we handle it ourselves?
			(this->*Handlers[opcode].handler)(*pck);
		else
			Log.Warning("Session", "Received unhandled packet with opcode (0x%.4X)", opcode);

		delete pck;
	}
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void Session::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
	uint32 t = (uint32)UNIXTIME;
	OutPacket(SMSG_QUERY_TIME_RESPONSE, 4, &t);
}
