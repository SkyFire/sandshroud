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

typedef HM_NAMESPACE::hash_map<uint32, PlayerCreateInfo*> PlayerCreateInfoMap;

class ClientMgr : public Singleton<ClientMgr>
{
public:
	typedef HM_NAMESPACE::hash_map<uint32, RPlayerInfo*> ClientMap;
	ClientMap m_clients;
	RWLock m_lock;

protected:
	uint32 m_hiPlayerGuid;
	uint32 m_hiItemGuid;
	uint32 m_maxSessionId;

	Session * m_sessions[MAX_SESSIONS];
	HM_NAMESPACE::hash_map<RPlayerInfo*, Session*> m_sessionsbyinfo;
	std::vector<uint32> m_reusablesessions;
	std::vector<uint32> m_pendingdeletesessionids;

public:
	ClientMgr();
	~ClientMgr();
	
	/* create rplayerinfo struct */
	RPlayerInfo * CreateRPlayer(uint32 guid);

	/* destroy rplayerinfo struct */
	void DestroyRPlayerInfo(uint32 guid);

	HEARTHSTONE_INLINE Session* GetSessionByRPInfo(RPlayerInfo* p)
	{
		m_lock.AcquireReadLock();
		HM_NAMESPACE::hash_map<RPlayerInfo*, Session*>::iterator itr = m_sessionsbyinfo.find(p);
		if (itr == m_sessionsbyinfo.end())
		{
			m_lock.ReleaseReadLock();
			return NULL;
		}
		Session* s = itr->second;
		m_lock.ReleaseReadLock();
		return s;
	}

	HEARTHSTONE_INLINE void AddSessionRPInfo(Session* s, RPlayerInfo* p)
	{
		m_lock.AcquireWriteLock();
		m_sessionsbyinfo.insert(std::make_pair<RPlayerInfo*, Session*>(p, s));
		m_lock.ReleaseWriteLock();
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
		if(Id < MAX_SESSIONS)
			return m_sessions[Id];
		return NULL;
	}

	HEARTHSTONE_INLINE Session* GetSessionByAccountId(uint32 Id)
	{
		for(uint32 id = 0; id < MAX_SESSIONS; id++)
		{
			if(m_sessions[Id] && m_sessions[Id]->GetAccountId() == Id)
				return m_sessions[Id];
		}
		return NULL;
	}

	/* create a new session, returns null if the player is already logged in */
	Session * CreateSession(uint32 AccountId);

	void DestroySession(uint32 sessionid);

	/* updates sessions */
	void Update();

public: // PlayerCreateInfo
	void LoadPlayerCreateInfo();
	PlayerCreateInfoMap mPlayerCreateInfo;
	PlayerCreateInfo* GetPlayerCreateInfo(uint8 race, uint8 class_) const;

public: // Player Creation and deletion/rename shit
	uint32 GeneratePlayerGuid() { return ++m_hiPlayerGuid; };
	int CreateNewPlayer(Session* session, WorldPacket& data);
};

#define sClientMgr ClientMgr::getSingleton()

#endif // CLIENT_MANAGER_H
