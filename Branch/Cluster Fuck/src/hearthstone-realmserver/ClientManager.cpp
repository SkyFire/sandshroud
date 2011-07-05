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

#ifdef WIN32
static HANDLE m_abortEventClientMgr = INVALID_HANDLE_VALUE;
#else
static pthread_cond_t abortcondClientMgr;
static pthread_mutex_t abortmutex;
#endif

ClientMgr::ClientMgr()
{
	Session::InitHandlers();
	m_threadRunning = true;
	m_SessionCount = 0;
	memset(&SNContainer, 0, SN_MAX * sizeof(uint32));
	memset(m_sessions, 0, MAX_SESSIONS * sizeof(Session*));
	Log.Success("ClientMgr", "Interface Created");

	QueryResult *result = CharacterDatabase.Query( "SELECT MAX(guid) FROM playeritems" );
	if( result )
	{
		SNContainer[SN_ITEM_GUID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	result = CharacterDatabase.Query( "SELECT MAX(guid) FROM characters" );
	if( result )
	{
		SNContainer[SN_PLAYER_GUID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	result = CharacterDatabase.Query( "SELECT MAX(guid) FROM corpses" );
	if( result )
	{
		SNContainer[SN_CORPSE_GUID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	result = WorldDatabase.Query("SELECT MAX(id) FROM creature_spawns");
	if( result )
	{
		SNContainer[SN_CREATURE_SPAWNID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	result = WorldDatabase.Query("SELECT MAX(id) FROM gameobject_spawns");
	if( result )
	{
		SNContainer[SN_GAMEOBJECT_SPAWNID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	result = CharacterDatabase.Query("SELECT MAX(group_id) FROM groups");
	if( result )
	{
		SNContainer[SN_GROUP_ID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	result = CharacterDatabase.Query("SELECT MAX(guildId) FROM guilds");
	if( result )
	{
		SNContainer[SN_GUILD_ID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	result = CharacterDatabase.Query("SELECT MAX(charterId) FROM charters");
	if( result )
	{
		SNContainer[SN_CHARTER_ID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	result = CharacterDatabase.Query("SELECT MAX(guid) FROM gm_tickets");
	if( result )
	{
		SNContainer[SN_TICKET_ID] = result->Fetch()[0].GetUInt64();
		delete result;
		result = NULL;
	}

	result = CharacterDatabase.Query( "SELECT MAX(guid) FROM characters" );
	if( result )
	{
		SNContainer[SN_EQUIPMENTSET_ID] = result->Fetch()[0].GetUInt32();
		delete result;
		result = NULL;
	}

	Log.Notice("ObjectMgr", "HighGuid(ITEM) = %u", SNContainer[SN_ITEM_GUID]);
	Log.Notice("ObjectMgr", "HighGuid(PLAYER) = %u", SNContainer[SN_PLAYER_GUID]);
	Log.Notice("ObjectMgr", "HighGuid(CORPSE) = %u", SNContainer[SN_CORPSE_GUID]);
	Log.Notice("ObjectMgr", "HighGuid(CONTAINER) = %u", SNContainer[SN_CONTAINER_GUID]);
	Log.Notice("ObjectMgr", "HighGuid(UNIT) = %u", SNContainer[SN_CREATURE_SPAWNID]);
	Log.Notice("ObjectMgr", "HighGuid(GAMEOBJ) = %u", SNContainer[SN_GAMEOBJECT_SPAWNID]);
	Log.Notice("ObjectMgr", "HighGuid(GROUP) = %u", SNContainer[SN_GROUP_ID]);
	Log.Notice("ObjectMgr", "HighGuid(GUILD) = %u", SNContainer[SN_GUILD_ID]);
	Log.Notice("ObjectMgr", "HighGuid(CHARTER) = %u", SNContainer[SN_CHARTER_ID]);
	Log.Notice("ObjectMgr", "HighGuid(TICKET) = %u", SNContainer[SN_TICKET_ID]);
	Log.Notice("ObjectMgr", "HighGuid(EQSETS) = %u", SNContainer[SN_EQUIPMENTSET_ID]);
	LoadPlayerCreateInfo();
}

ClientMgr::~ClientMgr()
{
#ifdef WIN32
	CloseHandle(m_abortEventClientMgr);
#else
	pthread_cond_destroy(&abortcondClientMgr);
	pthread_mutex_destroy(&abortmutex);
#endif
};

void ClientMgr::terminate()
{
	m_threadRunning = false;
#ifdef WIN32
	SetEvent(m_abortEventClientMgr);
#else
	pthread_cond_signal(&abortcondClientMgr);
#endif
}

bool ClientMgr::run()
{
	m_threadRunning = true;
	while(m_threadRunning)
	{
		Update();
#ifdef WIN32
		WaitForSingleObject(m_abortEventClientMgr, 5);
#else
		Sleep(5);
#endif
	}
	return true;
}

void ClientMgr::SendPackedClientInfo(WServer * server)
{
	if(!m_clients.size())
		return;

	WorldPacket data(SMSGR_PACKED_PLAYER_INFO, sizeof(RPlayerInfo) * m_clients.size() + 4);
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
	//session doesn't exist
	Session* s = GetSession(sessionid);
	if (s == NULL)
		return;

	sessionLock.Acquire();
	s->deleted = true;
	m_pendingdeletesessionids.push_back(sessionid);
	sessionLock.Release();
}

Session * ClientMgr::CreateSession(uint32 AccountId)
{
	sessionLock.Acquire();
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
		sessionid = ++m_SessionCount;
		Log.Debug("Session", "New max session id: %u", sessionid);
	}

	//ok, if we have a session with this account, add it to delete queue
	if(GetSessionByAccountId(AccountId))
		m_pendingdeletesessionids.push_back(AccountId);

	//we couldn't generate an id for some reason
	if(sessionid == 0)
	{
		sessionLock.Release();
		return NULL;
	}

	Log.Debug("ClientMgr", "Allocating session %u for account id %u", sessionid, AccountId);
	Session *s = new Session(sessionid);

	m_sessions[sessionid] = s;
	sessionLock.Release();
	return s;
}

void ClientMgr::Update()
{
	sessionLock.Acquire();
	for(uint32 i = 0; i <= m_SessionCount; ++i)
		if(m_sessions[i])
			m_sessions[i]->Update();
	sessionLock.Release();
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

void ClientMgr::LoadPlayerCreateInfo()
{
	QueryResult *result = WorldDatabase.Query( "SELECT * FROM playercreateinfo" );

	if( result == NULL )
	{
		Log.Error("MySQL","Query failed: SELECT * FROM playercreateinfo");
		return;
	}

	if( result->GetFieldCount() < 25 )
	{
		Log.Error("PlayerCreateInfo", "Incorrect number of columns in playercreateinfo found %u, should be 25. check for sql updates", result->GetFieldCount());
		delete result;
		return;
	}

	PlayerCreateInfo *pPlayerCreateInfo;
	int fieldcount = 0;

	do
	{
		Field *fields = result->Fetch();
		fieldcount = 0;

		pPlayerCreateInfo = new PlayerCreateInfo;
		pPlayerCreateInfo->index = fields[fieldcount++].GetUInt8();
		pPlayerCreateInfo->race = fields[fieldcount++].GetUInt8();
		pPlayerCreateInfo->factiontemplate = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->class_ = fields[fieldcount++].GetUInt8();
		pPlayerCreateInfo->mapId = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->zoneId = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->positionX = fields[fieldcount++].GetFloat();
		pPlayerCreateInfo->positionY = fields[fieldcount++].GetFloat();
		pPlayerCreateInfo->positionZ = fields[fieldcount++].GetFloat();
		pPlayerCreateInfo->Orientation = fields[fieldcount++].GetFloat();
		pPlayerCreateInfo->displayId = fields[fieldcount++].GetUInt16();
		pPlayerCreateInfo->strength = fields[fieldcount++].GetUInt8();
		pPlayerCreateInfo->ability = fields[fieldcount++].GetUInt8();
		pPlayerCreateInfo->stamina = fields[fieldcount++].GetUInt8();
		pPlayerCreateInfo->intellect = fields[fieldcount++].GetUInt8();
		pPlayerCreateInfo->spirit = fields[fieldcount++].GetUInt8();
		pPlayerCreateInfo->health = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->mana = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->rage = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->focus = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->energy = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->runic = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->attackpower = fields[fieldcount++].GetUInt32();
		pPlayerCreateInfo->mindmg = fields[fieldcount++].GetFloat();
		pPlayerCreateInfo->maxdmg = fields[fieldcount++].GetFloat();

		QueryResult *sk_sql = WorldDatabase.Query("SELECT * FROM playercreateinfo_skills WHERE indexid = %u", pPlayerCreateInfo->index);
		if(sk_sql)
		{
			do
			{
				Field *fields = sk_sql->Fetch();
				CreateInfo_SkillStruct tsk;
				tsk.skillid = fields[1].GetUInt32();
				tsk.currentval = fields[2].GetUInt32();
				tsk.maxval = fields[3].GetUInt32();
				pPlayerCreateInfo->skills.push_back(tsk);
			} while(sk_sql->NextRow());
			delete sk_sql;
		}

		QueryResult *sp_sql = WorldDatabase.Query("SELECT * FROM playercreateinfo_spells WHERE indexid = %u", pPlayerCreateInfo->index);
		if(sp_sql)
		{
			do
			{
				pPlayerCreateInfo->spell_list.insert(sp_sql->Fetch()[1].GetUInt32());
			} while(sp_sql->NextRow());
			delete sp_sql;
		}

		QueryResult *items_sql = WorldDatabase.Query("SELECT * FROM playercreateinfo_items WHERE indexid = %u", pPlayerCreateInfo->index);
		if(items_sql)
		{
			do
			{
				Field *fields = items_sql->Fetch();
				CreateInfo_ItemStruct itm;
				itm.protoid = fields[1].GetUInt32();
				itm.slot = fields[2].GetUInt8();
				itm.amount = fields[3].GetUInt32();
				pPlayerCreateInfo->items.push_back(itm);
			} while(items_sql->NextRow());
			delete items_sql;
		}

		QueryResult *bars_sql = WorldDatabase.Query("SELECT * FROM playercreateinfo_bars WHERE class = %u",pPlayerCreateInfo->class_ );
		if(bars_sql)
		{
			do
			{
				Field *fields = bars_sql->Fetch();
				CreateInfo_ActionBarStruct bar;
				bar.button = fields[2].GetUInt32();
				bar.action = fields[3].GetUInt32();
				bar.type = fields[4].GetUInt32();
				bar.misc = fields[5].GetUInt32();
				pPlayerCreateInfo->actionbars.push_back(bar);
			} while(bars_sql->NextRow());
			delete bars_sql;
		}

		mPlayerCreateInfo[pPlayerCreateInfo->index] = pPlayerCreateInfo;
	} while( result->NextRow() );
	delete result;

	Log.Notice("ObjectMgr", "%u player create infos loaded.", mPlayerCreateInfo.size());
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

int ClientMgr::CreateNewPlayer(Session* session, WorldPacket& data)
{
	SNLock[SN_PLAYER_GUID].Acquire();
	if(SNContainer[SN_PLAYER_GUID]+1 == 0) // We've reset the count :O
	{
		SNLock[SN_PLAYER_GUID].Release();
		return 1;
	}

	DEBUG_LOG("ClientMgr", "Account(%u) creating a player", session->GetAccountId());
	Player* plr = new Player(++SNContainer[SN_PLAYER_GUID]);
	SNLock[SN_PLAYER_GUID].Release();

	uint8 error = plr->Create(session, data);
	if(error)
		return error;

	plr->SaveToDB(true);
	return 0;
}
