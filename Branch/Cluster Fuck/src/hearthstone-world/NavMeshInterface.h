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

#ifndef _NavMeshInterface_H
#define _NavMeshInterface_H

#define MMAP_MAGIC 0x4d4d4150	// 'MMAP'
#define MMAP_VERSION 3

struct MmapTileHeader
{
	uint32 mmapMagic;
	uint32 dtVersion;
	uint32 mmapVersion;
	uint32 size;
	bool usesLiquids : 1;

	MmapTileHeader() : mmapMagic(MMAP_MAGIC), dtVersion(DT_NAVMESH_VERSION),
		mmapVersion(MMAP_VERSION), size(0), usesLiquids(true) {}
};

// Crow: TODO: Integrate Cell Handler royalties.
class SERVER_DECL CNavMeshInterface
{
public:
	void Init();
	void DeInit();
	void LoadMap(uint32 mapid);

public: // Navmesh settings
	uint32 GetPosX(float x);
	uint32 GetPosY(float y);
	dtNavMesh* GetNavmesh(uint32 mapid);
	bool LoadNavMesh(uint32 mapid, uint32 x, uint32 y);
	bool IsNavmeshLoaded(uint32 mapid, uint32 x, uint32 y);
	bool IsNavmeshLoadedAtPosition(uint32 mapid, float x, float y);
	void UnloadNavMesh(uint32 mapid, uint32 x, uint32 y);

	LocationVector getBestPositionOnPathToLocation(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz);
	LocationVector getNextPositionOnPathToLocation(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz);

private:
	float calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y );

	dtNavMesh* m_navMesh[NUM_MAPS];
	uint32 internalX[NUM_MAPS][64];
	uint32 internalY[NUM_MAPS][64];
	int64 m_navMeshLoadCount[NUM_MAPS][64][64];
};

extern SERVER_DECL CNavMeshInterface NavMeshInterface;

#endif
