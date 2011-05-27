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

// Crow: TODO: Integrate Cell Handler royalties.
class SERVER_DECL CNavMeshInterface
{
public:
	void Init();
	void DeInit();

public: // Navmesh settings
	uint32 GetPosX(float x);
	uint32 GetPosY(float y);
	dtNavMesh* GetNavmesh(uint32 mapid);
	bool LoadNavMesh(uint32 mapid, uint32 x, uint32 y);
	bool IsNavmeshLoaded(uint32 mapid, uint32 x, uint32 y);
	bool IsNavmeshLoadedAtPosition(uint32 mapid, float x, float y);
	void UnloadNavMesh(uint32 mapid, uint32 x, uint32 y);
	LocationVector getNextPositionOnPathToLocation(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz);

private:
	dtNavMesh *m_navMesh[NUM_MAPS][64][64];
	int64 m_navMeshLoadCount[NUM_MAPS][64][64];
};

extern SERVER_DECL CNavMeshInterface NavMeshInterface;

#endif
