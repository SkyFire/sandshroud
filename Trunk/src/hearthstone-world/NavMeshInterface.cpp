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
	memset( MMaps, 0, sizeof(MMapManager*)*NUM_MAPS );
}

void CNavMeshInterface::DeInit()
{
	for(uint32 i = 0; i < NUM_MAPS; i++)
		delete MMaps[i];
}

MMapManager* CNavMeshInterface::GetOrCreateMMapManager(uint32 mapid)
{
	if(MMaps[mapid] != NULL)
		return MMaps[mapid];

	return (MMaps[mapid] = new MMapManager(mapid));
}

bool CNavMeshInterface::IsNavmeshLoaded(uint32 mapid, uint32 x, uint32 y)
{
	MMapManager* mmap = GetOrCreateMMapManager(mapid);
	return mmap->IsNavmeshLoaded(x, y);
}

bool CNavMeshInterface::LoadNavMesh(uint32 mapid, uint32 x, uint32 y)
{
	MMapManager* mmap = GetOrCreateMMapManager(mapid);
	return mmap->LoadNavMesh(x, y);
}

void CNavMeshInterface::UnloadNavMesh(uint32 mapid, uint32 x, uint32 y)
{
	MMapManager* mmap = GetOrCreateMMapManager(mapid);
	mmap->UnloadNavMesh(x, y);
}

bool CNavMeshInterface::BuildPath(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz, LocationVector& out)
{
	MMapManager* mmap = GetOrCreateMMapManager(mapid);
	return mmap->getNextPositionOnPathToLocation(startx, starty, startz, endx, endy, endz, out);
}

LocationVector CNavMeshInterface::BuildPath(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz, bool best)
{
	MMapManager* mmap = GetOrCreateMMapManager(mapid);
	if(best)
		return mmap->getBestPositionOnPathToLocation(startx, starty, startz, endx, endy, endz);
	return mmap->getNextPositionOnPathToLocation(startx, starty, startz, endx, endy, endz);
}

float CNavMeshInterface::GetWalkingHeight(uint32 mapid, float x, float y, float z, float z2)
{
	LocationVector Step;
	float height = MMAP_UNAVAILABLE;
	MMapManager* mmap = GetOrCreateMMapManager(mapid);
	if(mmap->GetWalkingHeightInternal(x, y, z, z2, Step))
		height = Step.z;
	return height;
}

MMapManager::MMapManager(uint32 mapid)
{
	lastTileRef = 0;
	m_navMesh = NULL;
	ManagerMapId = mapid;

	// load and init dtNavMesh - read parameters from file
	uint32 pathLen = uint32(sWorld.MMapPath.length() + strlen("/000.mmap")+1);
	char *fileName = new char[pathLen];
	snprintf(fileName, pathLen, (sWorld.MMapPath+"/%03i.mmap").c_str(), ManagerMapId);

	FILE* file = fopen(fileName, "rb");
	if (!file)
	{
		Log.Error("NavMeshInterface", "Could not load mmap %03i", mapid);
		delete [] fileName;
		return;
	}

	dtNavMeshParams params;
	fread(&params, sizeof(dtNavMeshParams), 1, file);
	fclose(file);

	dtNavMesh* mesh = mallocNavMesh();
	ASSERT(mesh);
	if(mesh->init(&params) != DT_SUCCESS)
	{
		freeNavMesh(mesh);
		Log.Error("NavMeshInterface", "Failed to initialize dtNavMesh for mmap %03u from file %s", mapid, fileName);
		delete [] fileName;
		return;
	}

	delete [] fileName;

	Log.Debug("NavMeshInterface", "Loaded %03i.mmap", mapid);

	// store inside our map list
	m_navMesh = mesh;
}

MMapManager::~MMapManager()
{
	delete m_navMesh;
}

float MMapManager::calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	float dx = Position2X-Position1X;
	float dy = Position2Y-Position1Y;
	double angle = 0.0f;

	// Calculate angle
	if (dx == 0.0)
	{
		if (dy == 0.0)
			angle = 0.0;
		else if (dy > 0.0)
			angle = M_PI * 0.5 /* / 2 */;
		else
			angle = M_PI * 3.0 * 0.5/* / 2 */;
	}
	else if (dy == 0.0)
	{
		if (dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if (dx < 0.0)
			angle = atanf(dy/dx) + M_PI;
		else if (dy < 0.0)
			angle = atanf(dy/dx) + (2*M_PI);
		else
			angle = atanf(dy/dx);
	}

	// Convert to degrees
	angle = angle * float(180 / M_PI);

	// Return
	return float(angle);
}

bool MMapManager::LoadNavMesh(uint32 x, uint32 y)
{
	if(m_navMesh == NULL)
		return false;

	uint32 PackedTileID = packTileID(x, y);
	dtTileRef reference = 0;

	ReferenceMap::iterator itr = TileReferences.find(PackedTileID);
	if(itr == TileReferences.end())
	{
		// load this tile :: mmaps/MMMXXYY.mmtile
		uint32 pathLen = uint32(sWorld.MMapPath.length() + strlen("/0000000.mmtile")+1);
		char *fileName = new char[pathLen];
		snprintf(fileName, pathLen, (sWorld.MMapPath+"/%03i%02i%02i.mmtile").c_str(), ManagerMapId, x, y);
		FILE *file = fopen(fileName, "rb");
		if (!file)
		{
			Log.Error("NavMeshInterface", "Could not open mmtile file '%s'", fileName);
			delete [] fileName;
			return false;
		}
		delete [] fileName;

		// read header
		MmapTileHeader fileHeader;
		fread(&fileHeader, sizeof(MmapTileHeader), 1, file);
		if (fileHeader.mmapMagic != MMAP_MAGIC)
		{
			Log.Error("NavMeshInterface", "Bad header in mmap %03u%02i%02i.mmtile", ManagerMapId, x, y);
			fclose(file);
			return false;
		}

		if (fileHeader.mmapVersion != MMAP_VERSION)
		{
			Log.Error("NavMeshInterface", "%03u%02i%02i.mmtile was built with generator v%i, expected v%i", ManagerMapId, x, y, fileHeader.mmapVersion, MMAP_VERSION);
			fclose(file);
			return false;
		}

		unsigned char* data = (unsigned char*)malloc(fileHeader.size);
		ASSERT(data);

		size_t result = fread(data, fileHeader.size, 1, file);
		if(!result)
		{
			Log.Error("NavMeshInterface", "Bad header or data in mmap %03u%02u%02u.mmtile", ManagerMapId, x, y);
			fclose(file);
			return false;
		}
		fclose(file);

		dtStatus dtresult;
		dtMeshHeader* header = (dtMeshHeader*)data;

		// memory allocated for data is now managed by detour, and will be deallocated when the tile is removed
		dtresult = m_navMesh->addTile(data, fileHeader.size, DT_TILE_FREE_DATA, 0, &reference);
		if(dtresult == DT_IN_PROGRESS) // We already have it loaded, oops.
			free(data);
		else if(dtresult == DT_FAILURE)
		{
			free(data);
			Log.Error("NavMeshInterface", "Could not load %03u%02u%02u.mmtile into navmesh", ManagerMapId, x, y);
			return false;
		}
		else Log.Debug("NavMeshInterface", "Loaded mmtile %03u[%04u] into %03u[%02u,%02u]", ManagerMapId, reference, ManagerMapId, x, y);

		TileReferences.insert(make_pair(PackedTileID, new TileReferenceC(reference)));
	}
	else reference = itr->second->ID;

	TileLoadCount[reference]++;
	return true;
}

void MMapManager::UnloadNavMesh(uint32 x, uint32 y)
{
	if(m_navMesh == NULL)
		return;

	uint32 PackedTileID = packTileID(x, y);
	ReferenceMap::iterator itr = TileReferences.find(PackedTileID);
	if(itr == TileReferences.end())
		return; // We aren't loaded, so why continue?

	dtTileRef reference = itr->second->ID;
	if(TileLoadCount[reference] == 1)
	{
		dtStatus status = m_navMesh->removeTile(reference, NULL, NULL);
		if(status == DT_FAILURE)
		{
			Log.Error("NavMeshInterface", "Failed to unload mmtile %03u[%04u] from %03u[%02u,%02u]", ManagerMapId, reference, ManagerMapId, x, y);
			return;
		}
		delete itr->second;
		TileReferences.erase(itr);
		Log.Debug("NavMeshInterface", "Unloaded mmtile %03u[%04i] from %03u[%02u,%02u]", ManagerMapId, reference, ManagerMapId, x, y);
	}

	TileLoadCount[reference]--;
}

bool MMapManager::IsNavmeshLoaded(uint32 x, uint32 y)
{
	uint32 PackedTileID = packTileID(x, y);
	ReferenceMap::iterator itr = TileReferences.find(PackedTileID);
	if(itr == TileReferences.end())
		return false;
	return true;
}

LocationVector MMapManager::getNextPositionOnPathToLocation(float startx, float starty, float startz, float endx, float endy, float endz)
{
	if(m_navMesh == NULL)
		return LocationVector(endx, endy, endz);

	dtNavMeshQuery* query = mallocNavMeshQuery();
	if(query->init(m_navMesh, 1024) != DT_SUCCESS)
	{
		freeNavMeshQuery(query);
		Log.Error("NavMeshInterface", "Failed to initialize dtNavMeshQuery for mapId %03u", ManagerMapId);
		return LocationVector(endx, endy, endz);
	}

	//convert to nav coords.
	float startPos[3] = { starty, startz, startx };
	float endPos[3] = { endy, endz, endx };
	float mPolyPickingExtents[3] = { 2.00f, 4.00f, 2.00f };
	float closestPoint[3] = {0.0f, 0.0f, 0.0f};
	int gx = GetPosX(startx)/8;
	int gy = GetPosY(starty)/8;
	LocationVector pos;
	pos.x = endx;
	pos.y = endy;
	pos.z = endz;
	dtStatus result;
	dtQueryFilter* mPathFilter = new dtQueryFilter();
	if(mPathFilter)
	{
		dtPolyRef mStartRef;
		result = query->findNearestPoly(startPos, mPolyPickingExtents, mPathFilter, &mStartRef, closestPoint);
		if(result != DT_SUCCESS || !mStartRef)
		{
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return pos;
		}

		dtPolyRef mEndRef;
		result = query->findNearestPoly(endPos, mPolyPickingExtents, mPathFilter, &mEndRef, closestPoint);
		if(result != DT_SUCCESS || !mEndRef)
		{
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return pos;
		}

		if (mStartRef != 0 && mEndRef != 0)
		{
			int mNumPathResults;
			dtPolyRef mPathResults[50];
			result = query->findPath(mStartRef, mEndRef,startPos, endPos, mPathFilter, mPathResults, &mNumPathResults, 50);
			if(result != DT_SUCCESS || mNumPathResults <= 0)
			{
				freeNavMeshQuery(query);
				delete mPathFilter;
				mPathFilter = NULL;
				return pos;
			}

			int mNumPathPoints;
			float actualpath[3*20];
			dtPolyRef polyrefs = 0;
			result = query->findStraightPath(startPos, endPos, mPathResults, mNumPathResults, actualpath, NULL, &polyrefs, &mNumPathPoints, 20);
			if (result != DT_SUCCESS || mNumPathPoints < 3)
			{
				freeNavMeshQuery(query);
				delete mPathFilter;
				mPathFilter = NULL;
				return pos;
			}

			pos.y = actualpath[3]; //0 3 6
			pos.z = actualpath[4]; //1 4 7
			pos.x = actualpath[5]; //2 5 8
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return pos;
		}
	}
	return pos;
}

bool MMapManager::getNextPositionOnPathToLocation(float startx, float starty, float startz, float endx, float endy, float endz, LocationVector& out)
{
	if(m_navMesh == NULL)
		return false;

	dtNavMeshQuery* query = mallocNavMeshQuery();
	if(query->init(m_navMesh, 1024) != DT_SUCCESS)
	{
		freeNavMeshQuery(query);
		Log.Error("NavMeshInterface", "Failed to initialize dtNavMeshQuery for mapId %03u", ManagerMapId);
		return false;
	}

	dtStatus result;
	//convert to nav coords.
	float startPos[3] = { starty, startz, startx };
	float endPos[3] = { endy, endz, endx };
	float mPolyPickingExtents[3] = { 2.00f, 4.00f, 2.00f };
	float closestPoint[3] = {0.0f, 0.0f, 0.0f};
	int gx = GetPosX(startx)/8;
	int gy = GetPosY(starty)/8;
	dtQueryFilter* mPathFilter = new dtQueryFilter();
	if(mPathFilter)
	{
		dtPolyRef mStartRef;
		result = query->findNearestPoly(startPos, mPolyPickingExtents, mPathFilter, &mStartRef, closestPoint);
		if(result != DT_SUCCESS || !mStartRef)
		{
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return false;
		}

		dtPolyRef mEndRef;
		result = query->findNearestPoly(endPos, mPolyPickingExtents, mPathFilter, &mEndRef, closestPoint);
		if(result != DT_SUCCESS || !mEndRef)
		{
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return false;
		}

		if (mStartRef != 0 && mEndRef != 0)
		{
			int mNumPathResults;
			dtPolyRef mPathResults[50];
			result = query->findPath(mStartRef, mEndRef,startPos, endPos, mPathFilter, mPathResults, &mNumPathResults, 50);
			if(result != DT_SUCCESS || mNumPathResults <= 0)
			{
				freeNavMeshQuery(query);
				delete mPathFilter;
				mPathFilter = NULL;
				return false;
			}

			int mNumPathPoints;
			float actualpath[3*20];
			dtPolyRef polyrefs = 0;
			result = query->findStraightPath(startPos, endPos, mPathResults, mNumPathResults, actualpath, NULL, &polyrefs, &mNumPathPoints, 20);
			if (result != DT_SUCCESS || mNumPathPoints < 3)
			{
				freeNavMeshQuery(query);
				delete mPathFilter;
				mPathFilter = NULL;
				return false;
			}

			out.y = actualpath[3]; //0 3 6
			out.z = actualpath[4]; //1 4 7
			out.x = actualpath[5]; //2 5 8
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return true;
		}
	}
	return false;
}

LocationVector MMapManager::getBestPositionOnPathToLocation(float startx, float starty, float startz, float endx, float endy, float endz)
{
	LocationVector pos(startx, starty, startz);
	LocationVector nextpos(startx, starty, startz);
	LocationVector returnpos(endx, endy, endz);
	pos = getNextPositionOnPathToLocation(startx, starty, startz, endx, endy, endz);
	float line = calcAngle(startx, starty, pos.x, pos.y);
	while(1)
	{
		nextpos = getNextPositionOnPathToLocation(pos.x, pos.y, pos.z, endx, endy, endz);
		float angle = calcAngle( startx, starty, nextpos.x, nextpos.y );
		if(angle != line)
		{	// We have to turn, so stop our line here.
			returnpos = pos;
			break;
		}
		if(pos.x == nextpos.x || pos.y == nextpos.y)
		{
			returnpos = pos;
			break;
		}

		pos = nextpos;
	}
	return returnpos;
}

bool MMapManager::GetWalkingHeightInternal(float positionx, float positiony, float positionz, float endz, LocationVector& out)
{
	if(m_navMesh == NULL)
		return false;

	dtNavMeshQuery* query = mallocNavMeshQuery();
	if(query->init(m_navMesh, 1024) != DT_SUCCESS)
	{
		freeNavMeshQuery(query);
		Log.Error("NavMeshInterface", "Failed to initialize dtNavMeshQuery for mapId %03u", ManagerMapId);
		return false;
	}

	dtStatus result;
	//convert to nav coords.
	float startPos[3] = { positionx, positiony, positionz };
	float endPos[3] = { positionx, positiony, endz };
	float mPolyPickingExtents[3] = { 2.00f, 2.00f, 4.00f };
	float closestPoint[3] = {0.0f, 0.0f, 0.0f};
	int gx = GetPosX(positionx)/8;
	int gy = GetPosY(positiony)/8;
	dtQueryFilter* mPathFilter = new dtQueryFilter();
	if(mPathFilter)
	{
		dtPolyRef mStartRef;
		result = query->findNearestPoly(startPos, mPolyPickingExtents, mPathFilter, &mStartRef, closestPoint);
		if(result != DT_SUCCESS || !mStartRef)
		{
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return false;
		}

		dtPolyRef mEndRef;
		result = query->findNearestPoly(endPos, mPolyPickingExtents, mPathFilter, &mEndRef, closestPoint);
		if(result != DT_SUCCESS || !mEndRef)
		{
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return false;
		}

		if (mStartRef != 0 && mEndRef != 0)
		{
			int mNumPathResults;
			dtPolyRef mPathResults[50];
			result = query->findPath(mStartRef, mEndRef,startPos, endPos, mPathFilter, mPathResults, &mNumPathResults, 50);
			if(result != DT_SUCCESS || mNumPathResults <= 0)
			{
				freeNavMeshQuery(query);
				delete mPathFilter;
				mPathFilter = NULL;
				return false;
			}

			int mNumPathPoints;
			float actualpath[3*2];
			dtPolyRef polyrefs = 0;
			result = query->findStraightPath(startPos, endPos, mPathResults, mNumPathResults, actualpath, NULL, &polyrefs, &mNumPathPoints, 2);
			if (result != DT_SUCCESS)
			{
				freeNavMeshQuery(query);
				delete mPathFilter;
				mPathFilter = NULL;
				return false;
			}

			if(mNumPathPoints < 3)
			{
				out.y = positiony;
				out.z = positionz;
				out.x = positionx;
				freeNavMeshQuery(query);
				delete mPathFilter;
				mPathFilter = NULL;
				return true;
			}

			out.y = actualpath[3];
			out.z = actualpath[4];
			out.x = actualpath[5];
			freeNavMeshQuery(query);
			delete mPathFilter;
			mPathFilter = NULL;
			return true;
		}
	}
	return false;
}
