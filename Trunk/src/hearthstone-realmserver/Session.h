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

#ifndef _R_SESSION_H
#define _R_SESSION_H

typedef void(Session::*SessionPacketHandler)(WorldPacket&);

class Session
{
public:
	friend class WorldSocket;
	friend class CharacterStorage;
	Session(uint32 id);
	~Session();

protected:
	FastQueue<WorldPacket*, Mutex> m_readQueue;
	WorldSocket * m_socket;
	WServer * m_server;
	WServer * m_nextServer;
	uint32 m_sessionId;
	uint32 m_accountId;
	RPlayerInfo * m_currentPlayer;
	uint32 m_latency;
	uint32 m_accountFlags;
	string m_GMPermissions;
	string m_accountName;
	uint32 m_build;
	uint32 m_muted;
	bool m_hasDeathKnight;
	uint8 m_highestLevel;
	static SessionPacketHandler Handlers[NUM_MSG_TYPES];
	bool m_loadedPlayerData;

public:
	static void InitHandlers();
	void Update();
	HEARTHSTONE_INLINE RPlayerInfo * GetPlayer() { return m_currentPlayer; }

	HEARTHSTONE_INLINE void ClearCurrentPlayer() { m_currentPlayer = 0; }
	HEARTHSTONE_INLINE void ClearServers() { m_nextServer = m_server = 0; }
	HEARTHSTONE_INLINE void SetNextServer() { m_server = m_nextServer; }
	HEARTHSTONE_INLINE void SetServer(WServer * s) { m_server = s; }
	HEARTHSTONE_INLINE WServer * GetServer() { return m_server; }
	HEARTHSTONE_INLINE WorldSocket * GetSocket() { return m_socket; }
	HEARTHSTONE_INLINE uint32 GetAccountId() { return m_accountId; }
	HEARTHSTONE_INLINE uint32 GetSessionId() { return m_sessionId; }

	void SendPacket(WorldPacket * data)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->SendPacket(data);
	}

	void HandlePlayerLogin(WorldPacket & pck);
	void HandleCharacterEnum(WorldPacket & pck);
	void HandleCharacterCreate(WorldPacket & pck);
	void HandleCharacterDelete(WorldPacket & pck);
	void HandleCharacterRename(WorldPacket & pck);
	void HandleRealmSplitQuery(WorldPacket & pck);
	void HandleItemQuerySingleOpcode(WorldPacket & pck);
	void HandleCreatureQueryOpcode(WorldPacket & pck);
	void HandleGameObjectQueryOpcode(WorldPacket & pck);
	void HandleItemPageQueryOpcode(WorldPacket & pck);
	void HandleNpcTextQueryOpcode(WorldPacket & pck);

	std::string GetAccountPermissions() { return m_GMPermissions; };
	std::string GetAccountName() { return m_accountName; };
};

#endif


