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

#ifndef WORKER_SERVER_H
#define WORKER_SERVER_H

#define MAX_SESSIONS_PER_SERVER 1000

struct Instance;
typedef void(WServer::*WServerHandler)(WorldPacket &);

class WServer
{
public:
	static WServerHandler PHandlers[MSGR_NUM_TYPES];
	uint32 m_id;
	WSSocket * m_socket;
	FastQueue<WorldPacket*, Mutex> m_recvQueue;
	list<Instance*> m_instances;

public:
	WServer(uint32 id, WSSocket * s);
	~WServer() { };
	static void InitHandlers();

	HEARTHSTONE_INLINE size_t GetInstanceCount() { return m_instances.size(); }
	HEARTHSTONE_INLINE void SendPacket(WorldPacket * data) { if(m_socket) m_socket->SendPacket(data); }
	HEARTHSTONE_INLINE void SendWoWPacket(Session * from, WorldPacket * data) { if(m_socket) m_socket->SendWoWPacket(from, data); }
	HEARTHSTONE_INLINE void AddInstance(Instance * pInstance) { m_instances.push_back(pInstance); }
	HEARTHSTONE_INLINE void QueuePacket(WorldPacket * data) { m_recvQueue.Push(data); }
	HEARTHSTONE_INLINE uint32 GetID() { return m_id; }

	void Update();
	bool Destructable() { return m_socket == NULL; };

protected:
	/* packet handlers */
	void HandleRegisterWorker(WorldPacket & pck);
	void HandleWoWPacket(WorldPacket & pck);
	void HandleNumberRequest(WorldPacket & pck);
	void HandlePlayerLoginResult(WorldPacket & pck);
	void HandlePlayerLogout(WorldPacket & pck);
	void HandleTeleportRequest(WorldPacket & pck);
	void HandleError(WorldPacket & pck);
	void HandleSwitchServer(WorldPacket & pck);
	void HandleCreatePlayerResult(WorldPacket & pck);
	void HandleTransporterMapChange(WorldPacket & pck);
	void HandleSaveAllPlayers(WorldPacket & pck);
	void HandlePlayerTeleport(WorldPacket & pck);
	void HandlePlayerInfo(WorldPacket & pck);
	void HandleChannelAction(WorldPacket & pck);
	void HandleChannelUpdate(WorldPacket & pck);
	void HandleChannelLFGDungeonStatusReply(WorldPacket& pck);
};

#endif // WORKER_SERVER_H
