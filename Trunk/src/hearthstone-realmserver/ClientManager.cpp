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

void ClientMgr::DestroySession(uint32 sessionid)
{
	m_lock.AcquireWriteLock();
	//session doesn't exist
	Session* s = GetSession(sessionid);
	if (s == NULL)
		return;

	s->deleted = true;
	m_pendingdeletesessionids.push_back(sessionid);
	m_lock.ReleaseWriteLock();
}

Session * ClientMgr::CreateSession(uint32 AccountId)
{
	//lets generate a session id
	//get from reusable
	uint32 sessionid = 0;
	if (m_reusablesessions.size() > 0)
	{
		sessionid = m_reusablesessions[m_reusablesessions.size() - 1];
		m_reusablesessions.pop_back();
	}
	else
	{
		sessionid = ++m_maxSessionId;
		Log.Debug("Session", "New max session id: %u", sessionid);
	}

	//ok, if we have a session with this account, add it to delete queue
	if(GetSessionByAccountId(AccountId))
		m_pendingdeletesessionids.push_back(AccountId);

	//we couldn't generate an id for some reason
	if(sessionid == 0)
		return NULL;

	Log.Debug("ClientMgr", "Allocating session %u for account id %u", sessionid, AccountId);
	Session* s = new Session(sessionid);

	m_sessions[sessionid] = s;
	return s;
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
