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

#ifndef REBIRTH_PATH_FINDER_H
#define REBIRTH_PATH_FINDER_H

class Object;

#define MESH_MAX_NODES 2048*64*64
#define MAX_PATH_LENGTH 256
#define VERTEX_SIZE 3

#define MAP_LIQUID_TYPE_NO_WATER    0x00
#define MAP_LIQUID_TYPE_WATER       0x01
#define MAP_LIQUID_TYPE_OCEAN       0x02
#define MAP_LIQUID_TYPE_MAGMA       0x04
#define MAP_LIQUID_TYPE_SLIME       0x08

#define MAP_ALL_LIQUIDS   (MAP_LIQUID_TYPE_WATER | MAP_LIQUID_TYPE_OCEAN | MAP_LIQUID_TYPE_MAGMA | MAP_LIQUID_TYPE_SLIME)

#define MAP_LIQUID_TYPE_DARK_WATER  0x10
#define MAP_LIQUID_TYPE_WMO_WATER   0x20

#define _USE_SMOOTH_PATH
#define MAX_SMOOTH_PATH_LENGTH  2048
#define SMOOTH_PATH_STEP_SIZE   8.0f
#define SMOOTH_PATH_SLOP        0.5f

#define INVALID_POLYREF 0

// see contrib/mmap/src/TileBuilder.h
enum NavTerrain
{
    NAV_GROUND  = 0x01,
    NAV_MAGMA   = 0x02,
    NAV_SLIME   = 0x04,
    NAV_WATER   = 0x08,
    NAV_UNUSED1 = 0x10,
    NAV_UNUSED2 = 0x20
    // we only have 6 bits of a bitfield
};

enum PathType
{
    PATHFIND_BLANK      = 0x0001,   // path not built yet
    PATHFIND_NORMAL     = 0x0002,   // normal path
    PATHFIND_SHORTCUT   = 0x0004,   // travel through obstacles, terrain, air, etc (old behavior)
    PATHFIND_INCOMPLETE = 0x0008,   // path is too long, more will be calculated later
    PATHFIND_NOPATH     = 0x0010    // could not find a path
};

class PathInfo
{
public:
    PathInfo(Object* from, const float x, const float y, const float z);
    ~PathInfo();

    HEARTHSTONE_INLINE void getStartPosition(float &x, float &y, float &z) { x = m_startPosition.x; y = m_startPosition.y; z = m_startPosition.z; }
    HEARTHSTONE_INLINE void getNextPosition(float &x, float &y, float &z) { x = m_nextPosition.x; y = m_nextPosition.y; z = m_nextPosition.z; }
    HEARTHSTONE_INLINE void getEndPosition(float &x, float &y, float &z) { x = m_endPosition.x; y = m_endPosition.y; z = m_endPosition.z; }

    HEARTHSTONE_INLINE PathNode getStartPosition() { return m_startPosition; }
    HEARTHSTONE_INLINE PathNode getNextPosition() { return m_nextPosition; }
    HEARTHSTONE_INLINE PathNode getEndPosition() { return m_endPosition; }

    HEARTHSTONE_INLINE uint32 getPathPointer() { return m_pointPathPointer == 0 ? 1 : m_pointPathPointer; }
    HEARTHSTONE_INLINE PointPath& getFullPath() { return m_pathPoints; }

    void Update(const float x, const float y, const float z);
    bool noPath();
    bool incompletePath();

    // only for debug
    dtNavMesh *getMesh(){ return m_navMesh;}
    dtNavMeshQuery *getMeshQuery(){ return m_navMeshQuery;}
    dtPolyRef *getPathPolyRef(){ return m_pathPolyRefs;}
    uint32 getPolyLength(){ return m_polyLength; }

private:
    dtPolyRef   *   m_pathPolyRefs;     // array of detour polygon references
    uint32          m_polyLength;       // number of polygons in the path

    PointPath       m_pathPoints;       // our actual (x,y,z) path to the target
    uint32          m_pointPathPointer; // points to current triple in m_pathPoints - used when dest do not change
    // the triple is the one that is currently being moved toward

    PathNode        m_startPosition;    // {x, y, z} of current location
    PathNode        m_nextPosition;     // {x, y, z} of next location on the path
    PathNode        m_endPosition;      // {x, y, z} of the destination

    Object *        m_sourceObject;     // the object that is moving (safe pointer because PathInfo is only accessed from the mover?)
    dtNavMesh   *   m_navMesh;          // the nav mesh
    dtNavMeshQuery* m_navMeshQuery;     // the nav mesh query used to find the path
    PathType        m_type;             // tells what kind of path this is

    HEARTHSTONE_INLINE void setNextPosition(PathNode point) { m_nextPosition = point; }
    HEARTHSTONE_INLINE void setStartPosition(PathNode point) { m_startPosition = point; }
    HEARTHSTONE_INLINE void setEndPosition(PathNode point) { m_endPosition = point; }

    HEARTHSTONE_INLINE void clear()
    {
        delete [] m_pathPolyRefs;
        m_pathPolyRefs = NULL;

        m_polyLength = 0;

        m_pathPoints.clear();
        m_pointPathPointer = 0;
    }

    dtPolyRef getPathPolyByPosition(float x, float y, float z);

    void BuildFreshPath();
    void BuildPath(dtPolyRef startPoly, float* startPos, dtPolyRef endPoly, float* endPos);
    // NODE: startPos, endPos is in Y,Z,X format!

    void updateNextPosition();
    void shortcut();

    dtQueryFilter createFilter();

    bool canFly();
    bool canSwim();
    NavTerrain getNavTerrain(float x, float y, float z);

    // smooth path functions
    uint32 fixupCorridor(dtPolyRef* path, const uint32 npath, const uint32 maxPath,
        const dtPolyRef* visited, const uint32 nvisited);
    bool getSteerTarget(const float* startPos, const float* endPos, const float minTargetDist,
        const dtPolyRef* path, const uint32 pathSize, float* steerPos,
        unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
        float* outPoints = 0, uint32* outPountcount = 0);

    uint32 findSmoothPath(const float* startPos, const float* endPos,
        const dtPolyRef* path, const uint32 pathSize,
        float* smoothPath, const uint32 smoothPathMaxSize);
};

HEARTHSTONE_INLINE bool inRangeYZX(const float* v1, const float* v2, const float r, const float h)
{
    const float dx = v2[0] - v1[0];
    const float dy = v2[1] - v1[1]; // elevation
    const float dz = v2[2] - v1[2];
    return (dx*dx + dz*dz) < r*r && fabsf(dy) < h;
}

HEARTHSTONE_INLINE bool inRange(const PathNode p1, const PathNode p2,
                    const float r, const float h)
{
    const float dx = p2.x - p1.x;
    const float dy = p2.y - p1.y;
    const float dz = p2.z - p1.z;
    return (dx*dx + dy*dy) < r*r && fabsf(dz) < h;
}

#endif
