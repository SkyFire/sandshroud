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

#define MMAP_UNAVAILABLE 424242.42f // 'MMAP'
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

struct TileReferenceC
{
	TileReferenceC(dtTileRef refid) { ID = refid; };
	~TileReferenceC() {};

	dtTileRef ID;
};

typedef map<uint32, TileReferenceC*> ReferenceMap;
typedef map<dtTileRef, uint32> ReverseReferenceMap;

class MMapManager
{
	uint32 GetPosX(float x)
	{
		ASSERT((x >= _minX) && (x <= _maxX));
		return (uint32)((_maxX-x)/_cellSize);
	};

	uint32 GetPosY(float y)
	{
		ASSERT((y >= _minY) && (y <= _maxY));
		return (uint32)((_maxY-y)/_cellSize);
	};
public:
	MMapManager(uint32 mapid);
	~MMapManager();

private:
	uint32 ManagerMapId;
	dtNavMesh* m_navMesh;
	dtTileRef lastTileRef;
	ReferenceMap TileReferences;
	ReverseReferenceMap TileLoadCount;
	uint32 packTileID(int32 x, int32 y) { return uint32(x << 16 | y); };
	float calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y );

public:
	bool LoadNavMesh(uint32 x, uint32 y);
	void UnloadNavMesh(uint32 x, uint32 y);
	bool IsNavmeshLoaded(uint32 x, uint32 y);

	LocationVector getNextPositionOnPathToLocation(float startx, float starty, float startz, float endx, float endy, float endz);
	LocationVector getBestPositionOnPathToLocation(float startx, float starty, float startz, float endx, float endy, float endz);

	bool GetWalkingHeightInternal(float startx, float starty, float startz, float endz, LocationVector& out);
	bool getNextPositionOnPathToLocation(float startx, float starty, float startz, float endx, float endy, float endz, LocationVector& out);
};

class SERVER_DECL CNavMeshInterface
{
public:
	void Init();
	void DeInit();
	MMapManager* GetOrCreateMMapManager(uint32 mapid);

public: // Navmesh settings
	bool LoadNavMesh(uint32 mapid, uint32 x, uint32 y);
	void UnloadNavMesh(uint32 mapid, uint32 x, uint32 y);
	bool IsNavmeshLoaded(uint32 mapid, uint32 x, uint32 y);
	bool IsNavmeshLoadedAtPosition(uint32 mapid, float x, float y) { return IsNavmeshLoaded(mapid, (GetPosX(x)/8), (GetPosY(y)/8)); };

	float GetWalkingHeight(uint32 mapid, float positionx, float positiony, float positionz, float positionz2);
	bool BuildPath(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz, LocationVector& out);
	LocationVector BuildPath(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz, bool best = false);

private:
	uint32 GetPosX(float x)
	{
		ASSERT((x >= _minX) && (x <= _maxX));
		return (uint32)((_maxX-x)/_cellSize);
	};

	uint32 GetPosY(float y)
	{
		ASSERT((y >= _minY) && (y <= _maxY));
		return (uint32)((_maxY-y)/_cellSize);
	};

	MMapManager* GetMMap(uint32 mapid)
	{
		MMapManager* mmapreturn = NULL;
		if(mapid < NUM_MAPS)
			mmapreturn = MMaps[mapid];
		return mmapreturn;
	};

	MMapManager* MMaps[NUM_MAPS];
};

extern SERVER_DECL CNavMeshInterface NavMeshInterface;

#endif
