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

class SERVER_DECL CNavMeshInterface
{
public:
    void Init();
    void DeInit();

public: // Navmesh settings
    dtNavMesh* GetNavmesh(uint32 mapid);
    bool LoadNavMesh(uint32 mapid, uint32 x, uint32 y);
    bool IsNavmeshLoaded(uint32 mapid, uint32 x, uint32 y);
    void UnloadNavMesh(uint32 mapid, uint32 x, uint32 y);
    // DEPRECATED
//    LocationVector getNextPositionOnPathToLocation(uint32 mapid, float startx, float starty, float startz, float endx, float endy, float endz);

private:
    dtNavMesh *m_navMesh[NUM_MAPS];
    int64 m_navMeshLoadCount[NUM_MAPS][64][64];
};

HEARTHSTONE_INLINE uint32 packTileID(uint32 tileX, uint32 tileY)
{
    return tileX << 16 | tileY;
};

HEARTHSTONE_INLINE void unpackTileID(uint32 ID, uint32 &tileX, uint32 &tileY)
{
    tileX = ID >> 16;
    tileY = ID & 0xFF;
};

extern SERVER_DECL CNavMeshInterface NavMeshInterface;

#endif
