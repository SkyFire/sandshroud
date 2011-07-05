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

#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

// Crow: USE THIS AS OUR OBJECTMANAGER REPLACEMENT!

#define MAX_SESSIONS 3000

typedef HM_NAMESPACE::hash_map<uint32, RPlayerInfo*> ClientMap;
typedef HM_NAMESPACE::hash_map<uint32, PlayerCreateInfo*> PlayerCreateInfoMap;

enum SharedNumbers
{
	SN_ITEM_GUID			= 0,
	SN_PLAYER_GUID,
	SN_CORPSE_GUID,
	SN_CONTAINER_GUID,
	SN_CREATURE_SPAWNID,
	SN_GAMEOBJECT_SPAWNID,
	SN_GROUP_ID,
	SN_GUILD_ID,
	SN_CHARTER_ID,
	SN_TICKET_ID,
	SN_EQUIPMENTSET_ID,
	SN_MAX
};

class ClientMgr : public Singleton<ClientMgr>, public ThreadContext
{
public:
	ClientMgr();
	~ClientMgr();

	ClientMap m_clients;

protected:
	Mutex SNLock[SN_MAX];
	uint32 SNContainer[SN_MAX];

	Mutex sessionLock;
	uint32 m_SessionCount;
	Session *m_sessions[MAX_SESSIONS];
	HM_NAMESPACE::hash_map<RPlayerInfo*, Session*> m_sessionsbyinfo;
	std::vector<uint32> m_reusablesessions;
	std::vector<uint32> m_pendingdeletesessionids;

public:
	bool run();
	void terminate();
	bool m_threadRunning;

	HEARTHSTONE_INLINE uint32 GetSharedNumber(uint8 type)
	{
		uint32 number = 0;
		if(type < SN_MAX)
		{
			SNLock[type].Acquire();
			number = ++SNContainer[type];
			SNLock[type].Release();
		}
		return number;
	}

	/* create rplayerinfo struct */
	RPlayerInfo * CreateRPlayer(uint32 guid);

	/* destroy rplayerinfo struct */
	void DestroyRPlayerInfo(uint32 guid);

	HEARTHSTONE_INLINE Session* GetSessionByRPInfo(RPlayerInfo* p)
	{
		sessionLock.Acquire();
		HM_NAMESPACE::hash_map<RPlayerInfo*, Session*>::iterator itr = m_sessionsbyinfo.find(p);
		if (itr == m_sessionsbyinfo.end())
		{
			sessionLock.Release();
			return NULL;
		}
		Session *s = itr->second;
		sessionLock.Release();
		return s;
	}

	HEARTHSTONE_INLINE void AddSessionRPInfo(Session* s, RPlayerInfo* p)
	{
		ASSERT(p && s);
		sessionLock.Acquire();
		m_sessionsbyinfo.insert(std::make_pair<RPlayerInfo*, Session*>(p, s));
		sessionLock.Release();
	}

	/* get rplayer */
	HEARTHSTONE_INLINE RPlayerInfo * GetRPlayer(uint32 guid)
	{
		ClientMap::iterator itr = m_clients.find(guid);
		return (itr != m_clients.end()) ? itr->second : 0;
	}

	/* get rplayer */
	HEARTHSTONE_INLINE RPlayerInfo * GetRPlayerByName(const char* name)
	{
		string lpn = HEARTHSTONE_TOLOWER_RETURN(string(name));
		ClientMap::iterator i;
		RPlayerInfo *rv = NULL;

		i = m_clients.begin();
		while(i != m_clients.end())
		{
			if(HEARTHSTONE_TOLOWER_RETURN(i->second->Name) == lpn)
			{
				rv = i->second;
				break;
			}
		}

		return rv;
	};

	/* send "mini" client data to all servers */
	void SendPackedClientInfo(WServer * server);

	/* get session by id */
	HEARTHSTONE_INLINE Session * GetSession(uint32 Id)
	{
		Session *s = NULL;
		sessionLock.Acquire();
		if(Id < MAX_SESSIONS)
			s = m_sessions[Id];
		sessionLock.Release();
		return s;
	}

	HEARTHSTONE_INLINE Session* GetSessionByAccountId(uint32 Id)
	{
		Session *s = NULL;
		sessionLock.Acquire();
		for(uint32 id = 0; id < MAX_SESSIONS; id++)
		{
			if(m_sessions[id] && m_sessions[id]->GetAccountId() == Id)
			{
				s = m_sessions[id];
				break;
			}
		}
		sessionLock.Release();
		return s;
	}

	/* create a new session, returns null if the player is already logged in */
	Session *CreateSession(uint32 AccountId);

	void DestroySession(uint32 sessionid);

	/* updates sessions */
	void Update();

	uint32 GetSessionCount() { return uint32(m_clients.size()); };

public: // PlayerCreateInfo
	void LoadPlayerCreateInfo();
	PlayerCreateInfoMap mPlayerCreateInfo;
	PlayerCreateInfo* GetPlayerCreateInfo(uint8 race, uint8 class_) const;
	int CreateNewPlayer(Session* session, WorldPacket& data);
};

#define sClientMgr ClientMgr::getSingleton()

#endif // CLIENT_MANAGER_H
