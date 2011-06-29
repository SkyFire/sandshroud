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

#ifndef CLUSTER_MANAGER_H
#define CLUSTER_MANAGER_H

#define MAX_WORKER_SERVERS 100
#define MAX_SINGLE_MAPID 750

struct Instance
{
	uint32 MapId;
	uint32 MapCount; // Used for load balancing for things like battleground servers
	WServer* Server;
};

#define IS_INSTANCE(a) (WorldMapInfoStorage.LookupEntry(a) != NULL && !IS_MAIN_MAP_CHECK(a) && !IS_BATTLEGROUND_CHECK(a))

#define IS_MAIN_MAP_CHECK(a) (WorldMapInfoStorage.LookupEntry(a)->type == 0)
#define IS_BATTLEGROUND_CHECK(a) (WorldMapInfoStorage.LookupEntry(a)->type == 3)

#define IS_MAIN_MAP(a) (WorldMapInfoStorage.LookupEntry(a) != NULL && IS_MAIN_MAP_CHECK(a))
#define IS_BATTLEGROUND(a) (WorldMapInfoStorage.LookupEntry(a) != NULL && IS_BATTLEGROUND_CHECK(a))

typedef map<uint32, Instance*> InstanceMap;

class ClusterMgr : public Singleton<ClusterMgr>, public ThreadContext
{
public:
	ClusterMgr();
	bool run();
	void terminate();
	bool m_threadRunning;

private:
	RWLock m_lock;

	Mutex Slave_lock;
	std::set<WServer*> JunkServers;
	WServer *WorkerServers[MAX_WORKER_SERVERS];
	Instance *SingleInstanceMaps[MAX_SINGLE_MAPID];

	uint32 m_maxWorkerServer;

public:
	WServer *GetServerByMapId(uint32 MapId);
	Instance *GetInstanceByMapId(uint32 MapId);
	Instance *GetAnyInstance();

	void RemoveWServer(uint32 index);

	WServer * CreateWorkerServer(WSSocket * s);
	HEARTHSTONE_INLINE WServer * GetWorkerServer(uint32 Id) { return (Id < MAX_WORKER_SERVERS) ? WorkerServers[Id] : 0; }
	bool AllocateInitialInstances(WServer * server, map<uint32, uint32> maps);

	// find the worker server with the least load for the new instance
	WServer * GetWorkerServerForNewInstance();

	/* create new instance, or a main map */
	Instance * CreateInstance(uint32 MapId, WServer * server);

	/* distribute packet to all worker servers */
	HEARTHSTONE_INLINE void DistributePacketToAll(WorldPacket * data) { DistributePacketToAll(data, 0); }

	/* distribute packet to all worker server excluding one */
	void DistributePacketToAll(WorldPacket * data, WServer * exclude);

	/* loop */
	void Update();

	uint8 key[20];
};

#define sClusterMgr ClusterMgr::getSingleton()

#endif // CLUSTER_MANAGER_H
