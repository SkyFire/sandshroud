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

#include "StdAfx.h"

struct CollisionMap
{
	uint32 m_loadCount;
	uint32 m_tileLoadCount[64][64];
	RWLock m_lock;
};

SERVER_DECL CCollideInterface CollideInterface;
VMAP::VMapManager2* CollisionMgr;
CollisionMap *m_mapLocks[NUM_MAPS];
Mutex m_mapCreateLock;

//extern void* collision_init();
//extern void collision_shutdown();

void CCollideInterface::Init()
{
	Log.Notice("CollideInterface", "Init");
	CollisionMgr = new VMAP::VMapManager2;
	memset(m_mapLocks, 0, sizeof(CollisionMap*)*NUM_MAPS);
}

void CCollideInterface::ActivateMap(uint32 mapId)
{
	if( !CollisionMgr ) return;
	m_mapCreateLock.Acquire();
	if( m_mapLocks[mapId] == NULL )
	{
		m_mapLocks[mapId] = new CollisionMap;
		m_mapLocks[mapId]->m_loadCount = 1;
		memset(m_mapLocks[mapId]->m_tileLoadCount, 0, sizeof(uint32)*64*64);
	}
	else
		m_mapLocks[mapId]->m_loadCount++;

	m_mapCreateLock.Release();
}

void CCollideInterface::DeactivateMap(uint32 mapId)
{
	if( !CollisionMgr )
		return;
	m_mapCreateLock.Acquire();
	ASSERT(m_mapLocks[mapId] != NULL);
	--m_mapLocks[mapId]->m_loadCount;
	if( m_mapLocks[mapId]->m_loadCount == 0 )
	{
		// no instances using this anymore
		delete m_mapLocks[mapId];
		CollisionMgr->unloadMap(mapId);
		m_mapLocks[mapId] = NULL;
	}
	m_mapCreateLock.Release();
}

void CCollideInterface::ActivateTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
	ASSERT(m_mapLocks[mapId] != NULL);
	if( !CollisionMgr )
		return;
	// acquire write lock
	m_mapLocks[mapId]->m_lock.AcquireWriteLock();
	if( m_mapLocks[mapId]->m_tileLoadCount[tileX][tileY] == 0 )
		if(CollisionMgr->loadMap(sWorld.vMapPath.c_str(), mapId, tileX, tileY))
			OUT_DEBUG("Loading VMap [%u/%u] successful", tileX, tileY);
		else
			OUT_DEBUG("Loading VMap [%u/%u] unsuccessful", tileX, tileY);

	// increment count
	m_mapLocks[mapId]->m_tileLoadCount[tileX][tileY]++;

	// release lock
	m_mapLocks[mapId]->m_lock.ReleaseWriteLock();
}

void CCollideInterface::DeactivateTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
	ASSERT(m_mapLocks[mapId] != NULL);
	if( !CollisionMgr )
		return;

	// get write lock
	m_mapLocks[mapId]->m_lock.AcquireWriteLock();
	if( (--m_mapLocks[mapId]->m_tileLoadCount[tileX][tileY]) == 0 )
		CollisionMgr->unloadMap(mapId, tileX, tileY);

	// release write lock
	m_mapLocks[mapId]->m_lock.ReleaseWriteLock();
}

bool CCollideInterface::IsActiveTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
	ASSERT(m_mapLocks[mapId] != NULL);
	if( !CollisionMgr )
		return false;

	bool isactive = false;

	// acquire write lock
	m_mapLocks[mapId]->m_lock.AcquireWriteLock();
	if(m_mapLocks[mapId]->m_tileLoadCount[tileX][tileY])
		isactive = true;
	m_mapLocks[mapId]->m_lock.ReleaseWriteLock(); // release lock

	return isactive;
}

bool CCollideInterface::CheckLOS(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2)
{
	ASSERT(m_mapLocks[mapId] != NULL);

	// get read lock
	m_mapLocks[mapId]->m_lock.AcquireReadLock();

	// get data
	bool res = CollisionMgr ? CollisionMgr->isInLineOfSight(mapId, x1, y1, z1, x2, y2, z2) : true;

	// release write lock
	m_mapLocks[mapId]->m_lock.ReleaseReadLock();

	// return
	return res;
}

bool CCollideInterface::GetFirstPoint(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2, float & outx, float & outy, float & outz, float distmod)
{
	ASSERT(m_mapLocks[mapId] != NULL);

	// get read lock
	m_mapLocks[mapId]->m_lock.AcquireReadLock();

	// get data
	bool res = (CollisionMgr ? CollisionMgr->getObjectHitPos(mapId, x1, y1, z1, x2, y2, z2, outx, outy, outz, distmod) : false);

	// release write lock
	m_mapLocks[mapId]->m_lock.ReleaseReadLock();

	// return
	return res;
}

float CCollideInterface::GetHeight(uint32 mapId, float x, float y, float z)
{
	ASSERT(m_mapLocks[mapId] != NULL);

	// get read lock
	m_mapLocks[mapId]->m_lock.AcquireReadLock();

	// get data
	float res = CollisionMgr ? CollisionMgr->getHeight(mapId, x, y, z) : NO_WMO_HEIGHT;

	// release write lock
	m_mapLocks[mapId]->m_lock.ReleaseReadLock();

	// return
	return res;
}

/* Crow: Systematic calculations based on Mangos, a big thank you to them! */
bool CCollideInterface::IsIndoor(uint32 mapId, float x, float y, float z)
{
	ASSERT(m_mapLocks[mapId] != NULL);
	if(!CollisionMgr)
		return false;

	uint32 flags = 0;
	int32 adtId = 0, rootId = 0, groupid = 0;
	if(CollisionMgr->getAreaInfo(mapId, x, y, z, flags, adtId, rootId, groupid))
	{
		bool indoor = false;
		WMOAreaTableEntry * WMOEntry = GetWorldMapOverlayEntry(adtId, rootId, groupid);
		if(WMOEntry != NULL)
		{
			AreaTable* ate = dbcArea.LookupEntry(WMOEntry->adtId);
			if(ate != NULL)
			{
				if(ate->AreaFlags & AREA_OUTSIDE)
					return false;
				if(ate->AreaFlags & AREA_INSIDE)
					return true;
			}
		}

		if( flags != 0 )
			if(flags & VA_FLAG_INDOORS && !(flags & VA_FLAG_IN_CITY) && !(flags & VA_FLAG_OUTSIDE) && !(flags & VA_FLAG_IN_CITY2) && !(flags & VA_FLAG_IN_CITY3))
				indoor = true;

		if(WMOEntry != NULL)
		{
			if(WMOEntry->Flags & 4)
				return false;

			if((WMOEntry->Flags & 2) != 0)
				indoor = true;
		}

		return indoor;
	}

	return false; // If we have no info, then we are outside.
}

uint32 CCollideInterface::GetVmapAreaFlags(uint32 mapId, float x, float y, float z)
{
	ASSERT(m_mapLocks[mapId] != NULL);

	// get read lock
	m_mapLocks[mapId]->m_lock.AcquireReadLock();

	// get data
	uint32 flags = CollisionMgr ? CollisionMgr->GetVmapFlags(mapId, x, y, z) : 0; 

	// release write lock
	m_mapLocks[mapId]->m_lock.ReleaseReadLock();

	// return
	return flags;
}

void CCollideInterface::DeInit()
{
	// bleh.
}
