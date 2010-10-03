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

// CellHandler.h
uint32 CNavMeshInterface::GetPosX(float x)
{
	ASSERT((x >= _minX) && (x <= _maxX));
	return (uint32)((_maxX-x)/_cellSize);
}

uint32 CNavMeshInterface::GetPosY(float y)
{
	ASSERT((y >= _minY) && (y <= _maxY));
	return (uint32)((_maxY-y)/_cellSize);
}

dtNavMesh* CNavMeshInterface::GetNavmesh(uint32 mapid)
{
	return m_navMesh[mapid][0][0];
}

bool CNavMeshInterface::IsNavmeshLoadedAtPosition(uint32 mapid, float x, float y)
{
	int tileX = GetPosX(x)/8;
	int tileY = GetPosY(y)/8;
	return IsNavmeshLoaded(mapid, tileX, tileY);
}

bool CNavMeshInterface::IsNavmeshLoaded(uint32 mapid, uint32 x, uint32 y)
{
	return (m_navMeshLoadCount[mapid][x][y] > 0 ? true : false);
}

bool CNavMeshInterface::LoadNavMesh(uint32 mapid, uint32 x, uint32 y)
{
	if(m_navMeshLoadCount[mapid][x][y] < 1)
	{
		// map file name
		uint32 len = uint32(sizeof(sWorld.MMapPath)+strlen("/%03u%02u%02u.mmap")+1);
		char *tmp = new char[len];
		snprintf(tmp, len, (char *)(sWorld.MMapPath+"/%03u%02u%02u.mmap").c_str(), mapid, x, y);

		// woot no need for generation here: lets just load navmesh itself!
		ifstream nvmsh( tmp, ofstream::binary );
		if( nvmsh )
		{
			nvmsh.seekg(0,std::ifstream::end);
			int navDataSize = nvmsh.tellg();
			nvmsh.seekg(0);
			unsigned char *navData = new unsigned char[navDataSize];
			nvmsh.read((char*) (navData),navDataSize);
			nvmsh.close();
			m_navMesh[mapid][x][y] = new dtNavMesh;
			if(m_navMesh[mapid][x][y] == NULL)
			{
				delete [] navData;
				m_navMeshLoadCount[mapid][x][y] = false;
				return false;
			}

			if(!m_navMesh[mapid][x][y]->init(navData, navDataSize, true, 2048))
			{
				delete [] navData;
				m_navMeshLoadCount[mapid][x][y] = false;
				return false;
			}
		}
		else
		{
			delete[] tmp;
			m_navMeshLoadCount[mapid][x][y] = false;
			return false;
		}

		delete[] tmp;
	}

	m_navMeshLoadCount[mapid][x][y]++;
	return true;
}

void CNavMeshInterface::UnloadNavMesh(uint32 mapid, uint32 x, uint32 y)
{
	if((--m_navMeshLoadCount[mapid][x][y]) < 1)
	{
		delete m_navMesh[mapid][x][y];
		m_navMesh[mapid][x][y] = NULL;
	}
}

LocationVector CNavMeshInterface::getNextPositionOnPathToLocation(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz)
{
	//convert to nav coords.
	float startPos[3] = { starty, startz, startx };
	float endPos[3] = { endy, endz, endx };
	float mPolyPickingExtents[3] = { 2.00f, 4.00f, 2.00f };
	dtQueryFilter* mPathFilter = new dtQueryFilter();
	int gx = GetPosX(startx)/8;
	int gy = GetPosY(starty)/8;
	LocationVector pos;
	pos.x = endx;
	pos.y = endy;
	pos.z = endz;
	dtNavMesh* myNavMesh = m_navMesh[mapid][gx][gy];
	if(myNavMesh)
	{
		dtPolyRef mStartRef = myNavMesh->findNearestPoly(startPos,mPolyPickingExtents,mPathFilter,0); // this maybe should be saved on mob for later
		dtPolyRef mEndRef = myNavMesh->findNearestPoly(endPos,mPolyPickingExtents,mPathFilter,0); // saved on player? probably waste since player moves to much
		if (mStartRef != NULL && mEndRef != NULL)
		{
			dtPolyRef mPathResults[50];
			int mNumPathResults = myNavMesh->findPath(mStartRef, mEndRef,startPos, endPos, mPathFilter ,mPathResults,50);//TODO: CHANGE ME
			if(mNumPathResults <= 0)
				return pos;

			float actualpath[3*20];
			unsigned char* flags = 0;
			dtPolyRef* polyrefs = 0;
			int mNumPathPoints = myNavMesh->findStraightPath(startPos, endPos,mPathResults, mNumPathResults, actualpath, flags, polyrefs,20);
			if (mNumPathPoints < 3)
				return pos;

			pos.x = actualpath[5]; //0 3 6
			pos.y = actualpath[3]; //1 4 7
			pos.z = actualpath[4]; //2 5 8
			return pos;
		}
	}
	return pos;
}
