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

#ifndef _CLUSTERINTERFACE_H
#define _CLUSTERINTERFACE_H

#define MAX_SESSIONS 3000

class ClusterInterface;
typedef void(ClusterInterface::*ClusterInterfaceHandler)(WorldPacket&);

class ClusterInterface : public Singleton<ClusterInterface>
{
	WSClient * _clientSocket;
	FastQueue<WorldPacket*, Mutex> _pckQueue;
	time_t _lastConnectTime;
	WorldSession * _sessions[MAX_SESSIONS];
	bool m_connected;
	uint8 key[20];
	uint32 m_latency;
	Mutex m_mapMutex;

public:
	bool initializing;
	Mutex m_onlinePlayerMapMutex;
	typedef HM_NAMESPACE::hash_map<uint32,RPlayerInfo*> OnlinePlayerStorageMap;
	OnlinePlayerStorageMap _onlinePlayers;

	string GenerateVersionString();

	static ClusterInterfaceHandler PHandlers[MSGR_NUM_TYPES];
	static void InitHandlers();

	ClusterInterface();
	~ClusterInterface();

	void ForwardWoWPacket(uint16 opcode, uint32 size, const void * data, uint32 sessionid);
	void ConnectToRealmServer();

	RPlayerInfo * GetPlayer(uint32 guid)
	{
		OnlinePlayerStorageMap::iterator itr;
		m_onlinePlayerMapMutex.Acquire();
		itr = _onlinePlayers.find(guid);
		m_onlinePlayerMapMutex.Release();
		return (itr == _onlinePlayers.end()) ? 0 : itr->second;
	}

	HEARTHSTONE_INLINE WorldSession * GetSession(uint32 sid)
	{
		if(sid < MAX_SESSIONS && _sessions[sid])
			return _sessions[sid];
		return NULL;
	}

	void HandleServerError(WorldPacket & pck);
	void HandleSessionRemoved(WorldPacket & pck);
	void HandleTeleportResult(WorldPacket & pck);
	void HandleAuthRequest(WorldPacket & pck);
	void HandleAuthResult(WorldPacket & pck);
	void HandleRegisterResult(WorldPacket & pck);
	void HandleCreateInstance(WorldPacket & pck);
	void HandleDestroyInstance(WorldPacket & pck);
	void HandlePlayerLogin(WorldPacket & pck);
	void HandlePackedPlayerInfo(WorldPacket & pck);
	void HandlePlayerInfo(WorldPacket & pck);
	void HandleWoWPacket(WorldPacket & pck);
	void HandleSaveAllPlayers(WorldPacket & pck);
	void HandleTransporterMapChange(WorldPacket & pck);
	void HandlePlayerTeleport(WorldPacket & pck);
	void HandleDestroyPlayerInfo(WorldPacket & pck);
	void HandleChannelAction(WorldPacket & pck);
	void HandleChannelLFGDungeonStatusRequest(WorldPacket & pck);

	HEARTHSTONE_INLINE void QueuePacket(WorldPacket * pck) { _pckQueue.Push(pck); }

	void Update();
	void DestroySession(uint32 sid);
	void ConnectionDropped();
	void PlayerLoggedOut(uint32 sid, uint32 guid);

	HEARTHSTONE_INLINE void SendPacket(WorldPacket * data) { if(_clientSocket) _clientSocket->SendPacket(data); }
	HEARTHSTONE_INLINE void SetSocket(WSClient * s) { _clientSocket = s; }

	void RequestTransfer(Player* plr, uint32 MapId, uint32 InstanceId, LocationVector & vec);
};

#define sClusterInterface ClusterInterface::getSingleton()

#endif
