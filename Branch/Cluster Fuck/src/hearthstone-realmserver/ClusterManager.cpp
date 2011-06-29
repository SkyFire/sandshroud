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

initialiseSingleton(ClusterMgr);

#ifdef WIN32
static HANDLE m_abortEventClusterMgr = INVALID_HANDLE_VALUE;
#else
static pthread_cond_t abortcondClusterMgr;
static pthread_mutex_t abortmutex;
#endif

ClusterMgr::ClusterMgr()
{
	memset(SingleInstanceMaps, 0, sizeof(WServer*)*MAX_SINGLE_MAPID);
	memset(WorkerServers, 0, sizeof(WServer*)*MAX_WORKER_SERVERS);
	m_maxWorkerServer = 0;
	Log.Success("ClusterMgr", "Interface Created");

	string strkey;
	Config.ClusterConfig.GetString("RealmServer", "ConnectionPassword", &strkey);

	Sha1Hash k;
	k.UpdateData(strkey);
	k.Finalize();
	memcpy(key, k.GetDigest(), 20);
	m_threadRunning = true;

	WServer::InitHandlers();
}

void ClusterMgr::terminate()
{
	m_threadRunning = false;
#ifdef WIN32
	SetEvent(m_abortEventClusterMgr);
#else
	pthread_cond_signal(&abortcondClusterMgr);
#endif
}

bool ClusterMgr::run()
{
	m_threadRunning = true;
	while(m_threadRunning)
	{
		Update();
#ifdef WIN32
		WaitForSingleObject(m_abortEventClusterMgr, 10);
#else
		Sleep(10);
#endif
	}
	return true;
}

WServer * ClusterMgr::GetServerByMapId(uint32 MapId)
{
	ASSERT(SingleInstanceMaps[MapId]);
	return SingleInstanceMaps[MapId]->Server;
}

Instance * ClusterMgr::GetInstanceByMapId(uint32 MapId)
{
	return SingleInstanceMaps[MapId];
}

Instance* ClusterMgr::GetAnyInstance()
{
	//
	m_lock.AcquireReadLock();
	for (uint32 i=0; i<MAX_SINGLE_MAPID; ++i)
	{
		if (SingleInstanceMaps[i] != NULL)
		{
			m_lock.ReleaseReadLock();
			return SingleInstanceMaps[i];
		}
	}
	m_lock.ReleaseReadLock();
	return NULL;

}

void ClusterMgr::RemoveWServer(uint32 index)
{
	Slave_lock.Acquire();
	if(WorkerServers[index])
	{
		for(uint32 map = 0; map < MAX_SINGLE_MAPID; map++)
		{
			if(SingleInstanceMaps[map])
			{
				if(SingleInstanceMaps[map]->Server == WorkerServers[index])
				{
					SingleInstanceMaps[map]->Server = NULL;
					delete SingleInstanceMaps[map];
					SingleInstanceMaps[map] = NULL;
				}
			}
		}

		JunkServers.insert(WorkerServers[index]);
		WorkerServers[index] = NULL;
	}
	Slave_lock.Release();
}

WServer * ClusterMgr::CreateWorkerServer(WSSocket * s)
{
	Slave_lock.Acquire();
	uint32 i; /* find an id */
	for(i = 1; i < MAX_WORKER_SERVERS; i++)
		if(WorkerServers[i] == 0)
			break;

	if(i == MAX_WORKER_SERVERS)
	{
		Slave_lock.Release();
		return 0;		// No spaces
	}

	Log.Notice("ClusterMgr", "Allocating worker server %u to %s:%u", i, s->GetIP(), s->GetPort());
	WorkerServers[i] = new WServer(i, s);
	if(m_maxWorkerServer <= i)
		m_maxWorkerServer = i+1;

	Slave_lock.Release();
	return WorkerServers[i];
}

bool ClusterMgr::AllocateInitialInstances(WServer * server, map<uint32, uint32> maps)
{
	bool made = false;
	for(map<uint32, uint32>::iterator itr = maps.begin(); itr != maps.end(); itr++)
	{
		if(SingleInstanceMaps[itr->first] == NULL)
		{
			made = true;
			CreateInstance(itr->first, server);
			if(!itr->second)
				break;
		}
	}
	return made;
}

Instance * ClusterMgr::CreateInstance(uint32 MapId, WServer * server)
{
	Instance * pInstance = new Instance;
	pInstance->MapId = MapId;
	pInstance->Server = server;
	SingleInstanceMaps[MapId] = pInstance;

	/* tell the actual server to create the instance */
	WorldPacket data(SMSGR_CREATE_INSTANCE, 4);
	data << MapId;
	server->SendPacket(&data);
	server->AddInstance(pInstance);
	DEBUG_LOG("ClusterMgr", "Allocating map %u to server %u", MapId, server->GetID());
	return pInstance;
}

WServer * ClusterMgr::GetWorkerServerForNewInstance()
{
    WServer * lowest = 0;
	int32 lowest_load = -1;

	/* for now we'll just work with the instance count. in the future we might want to change this to
	   use cpu load instead. */
	Slave_lock.Acquire();
	for(uint32 i = 0; i < MAX_WORKER_SERVERS; ++i) {
		if(WorkerServers[i] != 0) {
			if((int32)WorkerServers[i]->GetInstanceCount() < lowest_load)
			{
				lowest = WorkerServers[i];
				lowest_load = int32(WorkerServers[i]->GetInstanceCount());
			}
		}
	}
	Slave_lock.Release();

	return lowest;
}

void ClusterMgr::Update()
{
	Slave_lock.Acquire();
	for(uint32 i = 1; i < m_maxWorkerServer; i++)
		if(WorkerServers[i])
			WorkerServers[i]->Update();

	for(std::set<WServer*>::iterator itr = JunkServers.begin(); itr != JunkServers.end(); itr++)
		if((*itr)->Destructable())
			delete (*itr);
	Slave_lock.Release();
}

void ClusterMgr::DistributePacketToAll(WorldPacket * data, WServer * exclude)
{
	Slave_lock.Acquire();
	for(uint32 i = 0; i < m_maxWorkerServer; i++)
		if(WorkerServers[i] && WorkerServers[i] != exclude)
			WorkerServers[i]->SendPacket(data);
	Slave_lock.Release();
}
