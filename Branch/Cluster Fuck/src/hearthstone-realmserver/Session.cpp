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

SessionPacketHandler Session::Handlers[NUM_MSG_TYPES];

void Session::InitHandlers()
{
	memset(Handlers, 0, sizeof(void*) * NUM_MSG_TYPES);
	Handlers[CMSG_NAME_QUERY]					= &Session::HandleNameQueryOpcode;
	Handlers[CMSG_PLAYER_LOGIN]					= &Session::HandlePlayerLogin;
	Handlers[CMSG_CHAR_ENUM]					= &Session::HandleCharacterEnum;
	Handlers[CMSG_CHAR_CREATE]					= &Session::HandleCharacterCreate;
	Handlers[CMSG_CHAR_DELETE]					= &Session::HandleCharacterDelete;
	Handlers[CMSG_CHAR_RENAME]					= &Session::HandleCharacterRename;
	Handlers[CMSG_CHAR_CUSTOMIZE]				= &Session::HandleCharacterCustomize;
	Handlers[CMSG_REALM_SPLIT]					= &Session::HandleRealmSplitQuery;
	Handlers[CMSG_QUERY_TIME]					= &Session::HandleQueryTimeOpcode;
	Handlers[CMSG_UPDATE_ACCOUNT_DATA]			= &Session::HandleUpdateAccountData;
	Handlers[CMSG_REQUEST_ACCOUNT_DATA]			= &Session::HandleRequestAccountData;
	Handlers[CMSG_READY_FOR_ACCOUNT_DATA_TIMES]	= &Session::HandleReadyForAccountDataTimes;
	Handlers[CMSG_VOICE_SESSION_ENABLE]			= &Session::HandleEnableMicrophoneOpcode;
	Handlers[CMSG_SET_ACTIVE_VOICE_CHANNEL]		= &Session::HandleVoiceChatQueryOpcode;

	// Crow: TODO, but not really. Each DB should hold the same info as far as this is concerned, but maybe things that are guid dependent.
/*	Handlers[CMSG_CREATURE_QUERY]				= &Session::HandleCreatureQueryOpcode;
	Handlers[CMSG_GAMEOBJECT_QUERY]				= &Session::HandleGameObjectQueryOpcode;
	Handlers[CMSG_ITEM_QUERY_SINGLE]			= &Session::HandleItemQuerySingleOpcode;
	Handlers[CMSG_PAGE_TEXT_QUERY]				= &Session::HandleItemPageQueryOpcode;
	Handlers[CMSG_NPC_TEXT_QUERY]				= &Session::HandleNpcTextQueryOpcode;*/
}

Session::Session(uint32 id) : m_sessionId(id)
{
	m_socket = NULL;
	m_server = NULL;
	m_nextServer = NULL;
	m_accountId = 0;
	m_ClientBuild = 0;
	m_currentPlayer = NULL;
	m_latency = 0;
	m_accountFlags = 0;
	m_language = 0;
	m_muted = 0;
	m_hasDeathKnight = false;
	m_highestLevel = 1;
	m_loadedPlayerData = false;
	deleted = false;

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
	if(m_socket != NULL)
		m_socket->UpdateQueuedPackets();

	WorldPacket * pck;
	uint16 opcode;
	while((pck = m_readQueue.Pop()))
	{
		opcode = pck->GetOpcode();

		if(Session::Handlers[opcode] != 0) // can we handle it ourselves?
			(this->*Session::Handlers[opcode])(*pck);
		else if(m_server)
			m_server->SendWoWPacket(this, pck);
		else
			Log.Warning("Session", "Received unhandled packet with opcode (0x%.4X)", opcode);

		delete pck;
	}
}
