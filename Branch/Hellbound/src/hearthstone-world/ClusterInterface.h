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

class ClusterInterface;
typedef void(ClusterInterface::*ClusterInterfaceHandler)(WorldPacket&);

class ClusterInterface : public Singleton<ClusterInterface>
{
#ifdef WIN32
	typedef HM_NAMESPACE::hash_map<string, string> ForcedPermissionMap;
#else
	typedef map<string, string> ForcedPermissionMap;
#endif

	FastQueue<WorldPacket*, Mutex> _pckQueue;
	time_t _lastConnectTime;
	WSClient* _clientSocket;
	Mutex updateMutex;
	Mutex pendingLock;
	bool m_connected;
	Mutex m_mapMutex;
	uint8 key[20];

	uint32 last_ping;
	uint32 last_pong;
	uint32 pingtime;
	uint32 m_latency;

public:
	ClusterInterface();
	~ClusterInterface();
	void Startup();
	void ConnectToRealmServer();

	Mutex m_onlinePlayerMapMutex;
	string GenerateVersionString();

	static ClusterInterfaceHandler PHandlers[IMSG_NUM_TYPES];
	static void InitHandlers();

	ForcedPermissionMap forced_permissions;
	const string* GetForcedPermissions(string& username);

	void HandlePlayerLogin(WorldPacket & pck);
	void HandleRegisterResult(WorldPacket & pck);
	void ForwardWoWPacket(uint16 opcode, uint32 size, const void * data, uint32 sessionid);
	void HandleAuthRequest(WorldPacket & pck);
	void HandleAuthResult(WorldPacket & pck);
	void HandleServerPing(WorldPacket & pck);
	void HandleServerPong(WorldPacket & pck);

	HEARTHSTONE_INLINE void QueuePacket(WorldPacket * pck) { _pckQueue.Push(pck); }

	void Update();
	void SendPing();
	void DestroySession(uint32 sid);
	void ConnectionDropped();
	void PlayerLoggedOut(uint32 sid, uint32 guid);

	HEARTHSTONE_INLINE void SendPacket(WorldPacket * data) { if(_clientSocket) _clientSocket->SendPacket(data); }
	HEARTHSTONE_INLINE void SetSocket(WSClient * s) { _clientSocket = s; }
};

#define sClusterInterface ClusterInterface::getSingleton()

#endif
