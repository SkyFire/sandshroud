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
	memset( m_navMesh, 0, sizeof(dtNavMesh*)*NUM_MAPS*64*64 );
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
	return LocationVector(endx, endy, endz);
}
