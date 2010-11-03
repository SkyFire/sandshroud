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

initialiseSingleton(ClientMgr);

ClientMgr::ClientMgr()
{
	Session::InitHandlers();
	m_maxSessionId = 0;
	m_hiPlayerGuid = 0;
	memset(m_sessions, 0, MAX_SESSIONS * sizeof(Session*));
	Log.Success("ClientMgr", "Interface Created");

	QueryResult *result = CharacterDatabase.Query( "SELECT MAX(guid) FROM characters" );
	if( result )
	{
		m_hiPlayerGuid = result->Fetch()[0].GetUInt32();
		delete result;
	}
}

ClientMgr::~ClientMgr()
{

};

void ClientMgr::SendPackedClientInfo(WServer * server)
{
	if(!m_clients.size())
		return;

	WorldPacket data(ISMSG_PACKED_PLAYER_INFO, sizeof(RPlayerInfo) * m_clients.size() + 4);
	data << uint32(m_clients.size());

	/* pack them all togther, w000t! */
	ClientMap::iterator itr = m_clients.begin();
	RPlayerInfo * pi;
	for(; itr != m_clients.end(); ++itr)
	{
		pi = itr->second;
		pi->Pack(data);
	}

	/* TODO: compress the packet */
	server->SendPacket(&data);
}

void ClientMgr::RemoveSession(Session * sess)
{
	for(uint32 i = 0; i < MAX_SESSIONS; ++i)
	{
		if( m_sessions[i] == sess )
		{
			delete sess;
			m_sessions[i] = NULL;
		}
	}
}

Session * ClientMgr::CreateSession(uint32 AccountId)
{
	uint32 i = 0;
	for(uint32 p = 0; p < MAX_SESSIONS; ++p)
	{
		if(m_sessions[p] && m_sessions[p]->GetAccountId() == AccountId)
			return m_sessions[p];
		else if( m_sessions[p] == NULL )
			i = p;
	}

	if(m_maxSessionId < i)
	{
		DEBUG_LOG("ClientMgr", "New max session id: %u", i);
		m_maxSessionId = i;
	}

	DEBUG_LOG("ClientMgr", "Allocating session %u for account id %u", i, AccountId);
	m_sessions[i] = new Session(i);
	return m_sessions[i];
}

void ClientMgr::Update()
{
	for(uint32 i = 0; i <= m_maxSessionId; ++i)
		if(m_sessions[i])
			m_sessions[i]->Update();
}

RPlayerInfo * ClientMgr::CreateRPlayer(uint32 guid)
{
	ASSERT(m_clients.find(guid) == m_clients.end());
	RPlayerInfo * rp = new RPlayerInfo;
	rp->Guid = guid;
	m_clients[guid] = rp;
	return rp;
}

void ClientMgr::DestroyRPlayerInfo(uint32 guid)
{
	RPlayerInfo * rp;
	ClientMap::iterator itr = m_clients.find(guid);
	if(itr != m_clients.end())
	{
		rp = itr->second;
		m_clients.erase(itr);
		delete rp;
	}
}

PlayerCreateInfo* ClientMgr::GetPlayerCreateInfo(uint8 race, uint8 class_) const
{
	PlayerCreateInfoMap::const_iterator itr;
	for (itr = mPlayerCreateInfo.begin(); itr != mPlayerCreateInfo.end(); itr++)
	{
		if( (itr->second->race == race) && (itr->second->class_ == class_) )
			return itr->second;
	}
	return NULL;
}

int ClientMgr::CreateNewPlayer()
{
	return 2; // Till this works, don't fuck up our available guids.
	if(m_hiPlayerGuid+1 == 0) // We've reset the count :O
		return 1;

	uint32 guid = GeneratePlayerGuid();
	return 0;
}
