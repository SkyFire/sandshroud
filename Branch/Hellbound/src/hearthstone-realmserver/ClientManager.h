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

// Crow: USE THIS AS OUR OBJECTMANAGER REPLACEMENT!

#define MAX_SESSIONS 3000

typedef HM_NAMESPACE::hash_map<uint32, PlayerCreateInfo*> PlayerCreateInfoMap;

class ClientMgr : public Singleton<ClientMgr>
{
public:
	RWLock m_lock;

protected:
	uint32 m_maxSessionId;

	Session * m_sessions[MAX_SESSIONS];
	std::vector<uint32> m_reusablesessions;
	std::vector<uint32> m_pendingdeletesessionids;

public:
	ClientMgr();
	~ClientMgr();

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

public: // Player Creation and deletion/rename shit
	int DeleteCharacter(Session* session, WorldPacket& data);
	int CreateNewPlayer(Session* session, WorldPacket& data);
	int RenameCharacter(Session* session, WorldPacket& data);
};

#define sClientMgr ClientMgr::getSingleton()
