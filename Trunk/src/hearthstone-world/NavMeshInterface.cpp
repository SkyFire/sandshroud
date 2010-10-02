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

SERVER_DECL CNavMeshInterface NavMeshInterface;

void CNavMeshInterface::Init()
{
	Log.Notice("NavMeshInterface", "Init");
	memset( m_navMesh, 0, sizeof(dtNavMesh*)*NUM_MAPS );
	memset( m_navMeshLoadCount, 0, sizeof(int64)*NUM_MAPS*64*64 );
}

void CNavMeshInterface::DeInit()
{
	// bleh.
}

dtNavMesh* CNavMeshInterface::GetNavmesh(uint32 mapid)
{
	return m_navMesh[mapid];
}

bool CNavMeshInterface::IsNavmeshLoaded(uint32 mapid, uint32 x, uint32 y)
{
	return (m_navMeshLoadCount[mapid][x][y] > 0 ? true : false);
}

bool CNavMeshInterface::LoadNavMesh(uint32 mapid, uint32 x, uint32 y)
{
	char fileName[512];
	FILE* file;
	if(m_navMesh[mapid] == NULL)
	{
		sprintf(fileName, "%s/%03i.mmap", sWorld.MMapPath.c_str(), mapid);
		file = fopen(fileName, "rb");

		if(!file)
		{
			DEBUG_LOG("NavMesh Interface", "Could not open mmap '%s'", fileName);
			return false;
		}

		dtNavMeshParams params;
		uint32 offset;
		fread(&params, sizeof(dtNavMeshParams), 1, file);
		fread(&offset, sizeof(uint32), 1, file);
		fclose(file);

		m_navMesh[mapid] = dtAllocNavMesh();
		if(!m_navMesh[mapid]->init(&params))
		{
			delete m_navMesh[mapid];
			m_navMesh[mapid] = NULL;
			DEBUG_LOG("NavMesh Interface", "%03u.mmap unsuccessfully loaded(1).", mapid);
			return false;
		}

		DEBUG_LOG("NavMesh Interface", "%03u.mmap successfully loaded.", mapid);
	}

	if(m_navMeshLoadCount[mapid][x][y] < 0)
	{
		m_navMeshLoadCount[mapid][x][y] = 0;

		// mmaps/0000000.mmtile
		sprintf(fileName, "%s/%03i%02i%02i.mmtile", sWorld.MMapPath.c_str(), mapid, x, y);
		file = fopen(fileName, "rb");

		if(!file)
		{
			DEBUG_LOG("NavMesh Interface", "Could not open mmtile '%s'", fileName);
			return false;
		}

		fseek(file, 0, SEEK_END);
		int length = ftell(file);
		fseek(file, 0, SEEK_SET);

		unsigned char* data =  (unsigned char*)dtAlloc(length, DT_ALLOC_PERM);
		fread(data, length, 1, file);
		fclose(file);

		dtMeshHeader* header = (dtMeshHeader*)data;
		if (header->magic != DT_NAVMESH_MAGIC)
		{
			DEBUG_LOG("NavMesh Interface", "%03u%02i%02i.mmtile has an invalid header", mapid, x, y);
			dtFree(data);
			return false;
		}
		if (header->version != DT_NAVMESH_VERSION)
		{
			DEBUG_LOG("NavMesh Interface", "%03u%02i%02i.mmtile was built with Detour v%i, expected v%i", mapid, x, y, header->version, DT_NAVMESH_VERSION);
			dtFree(data);
			return false;
		}

		if(!m_navMesh[mapid]->addTile(data, length, DT_TILE_FREE_DATA))
		{
			DEBUG_LOG("NavMesh Interface", "could not load %03u%02i%02i.mmtile into navmesh", mapid, x, y);
			dtFree(data);
			return false;
		}
		DEBUG_LOG("NavMesh Interface", "%03u%02u%02u.mmtile successfully loaded.", mapid, x, y);
	}

	m_navMeshLoadCount[mapid][x][y]++;
	return true;
}

void CNavMeshInterface::UnloadNavMesh(uint32 mapid, uint32 x, uint32 y)
{
	if(m_navMesh[mapid] == NULL)
		return;

	if(x == 0 && y == 0)
	{
        dtFreeNavMesh(m_navMesh[mapid]);
		m_navMesh[mapid] = NULL;
		return;
	}

	if(--m_navMeshLoadCount[mapid][x][y] < 0)
	{
		DEBUG_LOG("NavMesh Interface", "Unloading Navmesh at %03u%02u%02u", mapid, x, y);
		m_navMesh[mapid]->removeTile(m_navMesh[mapid]->getTileRefAt(x, y), NULL, NULL);
	}
}

// DEPRECATED
/*
LocationVector CNavMeshInterface::getNextPositionOnPathToLocation(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz)
{
	//convert to nav coords.
	float startPos[3] = { starty, startz, startx };
	float endPos[3] = { endy, endz, endx };
	float mPolyPickingExtents[3] = { 2.00f, 4.00f, 2.00f };
	dtQueryFilter* mPathFilter = new dtQueryFilter();
	int gx = 32 - (startx/533.333333f);
	int gy = 32 - (starty/533.333333f);
	LocationVector pos;
	pos.x = endx;
	pos.y = endy;
	pos.z = endz;

	if (m_navMesh[mapid] != NULL)
	{
		dtNavMeshQuery* m_navMeshQuery = dtAllocNavMeshQuery();
		m_navMeshQuery->init(m_navMesh[mapid], 2048*64*64);
		dtPolyRef mStartRef = m_navMeshQuery->findNearestPoly(startPos,mPolyPickingExtents,mPathFilter,0); // this maybe should be saved on mob for later
		dtPolyRef mEndRef = m_navMeshQuery->findNearestPoly(endPos,mPolyPickingExtents,mPathFilter,0); // saved on player? probably waste since player moves to much

		if (mStartRef != NULL && mEndRef != NULL)
		{
			dtPolyRef mPathResults[50];
			int mNumPathResults = m_navMeshQuery->findPath(mStartRef, mEndRef,startPos, endPos, mPathFilter ,mPathResults,50);
			if(mNumPathResults <= 0)
				return pos;

			float actualpath[3*20];
			unsigned char* flags = 0;
			dtPolyRef* polyrefs = 0;
			int mNumPathPoints = m_navMeshQuery->findStraightPath(startPos, endPos,mPathResults, mNumPathResults, actualpath, flags, polyrefs,20);
			if (mNumPathPoints < 3)
				return pos;

			pos.x = actualpath[5]; //0 3 6
			pos.y = actualpath[3]; //1 4 7
			pos.z = actualpath[4]; //2 5 8
			return pos;
		}
	}
	return pos;
}*/
