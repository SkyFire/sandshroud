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
	memset(m_sessions, 0, MAX_SESSIONS * sizeof(Session*));
	Log.Success("ClientMgr", "Interface Created");
}

ClientMgr::~ClientMgr()
{

};

void ClientMgr::DestroySession(uint32 sessionid)
{
	m_lock.AcquireWriteLock();
	//session doesn't exist
	Session* s = GetSession(sessionid);
	if (s == NULL)
	{
		m_lock.ReleaseWriteLock();
		return;
	}

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

int ClientMgr::CreateNewPlayer(Session* session, WorldPacket& data)
{
	DEBUG_LOG("ClientMgr", "Account(%u) creating a player", session->GetAccountId());
	MasterServer* MasterControl = sClusterMgr.GetMasterServer();
	if(MasterControl != NULL)
	{
		MasterControl->SendWoWPacket(session, &data);
		return 3;
	}
	return 1;
}

int ClientMgr::DeleteCharacter(Session* session, WorldPacket& data)
{
	DEBUG_LOG("ClientMgr", "Account(%u) deleting a player", session->GetAccountId());
	MasterServer* MasterControl = sClusterMgr.GetMasterServer();
	if(MasterControl != NULL)
	{
		MasterControl->SendWoWPacket(session, &data);
		return 0;
	}
	return 1;
}

int ClientMgr::RenameCharacter(Session* session, WorldPacket& data)
{
	MasterServer* MasterControl = sClusterMgr.GetMasterServer();
	if(MasterControl != NULL)
	{
		MasterControl->SendWoWPacket(session, &data);
		return 0;
	}
	return 1;
}
