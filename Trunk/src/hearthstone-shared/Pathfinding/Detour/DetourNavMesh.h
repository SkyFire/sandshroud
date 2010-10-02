/*
 * Sandshroud Hearthstone
 * Copyright (c) 2009 Mikko Mononen memon@inside.org
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

#ifndef DETOURNAVMESH_H
#define DETOURNAVMESH_H

#include "DetourAlloc.h"

// Reference to navigation polygon.
typedef unsigned int dtPolyRef;

// Reference to navigation mesh tile.
typedef unsigned int dtTileRef;

// Maximum number of vertices per navigation polygon.
static const int DT_VERTS_PER_POLYGON = 6;

static const int DT_NAVMESH_MAGIC = 'D'<<24 | 'N'<<16 | 'A'<<8 | 'V'; //'DNAV';
static const int DT_NAVMESH_VERSION = 4;

static const int DT_NAVMESH_STATE_MAGIC = 'D'<<24 | 'N'<<16 | 'M'<<8 | 'S'; //'DNMS';
static const int DT_NAVMESH_STATE_VERSION = 1;

static const unsigned short DT_EXT_LINK = 0x8000;
static const unsigned int DT_NULL_LINK = 0xffffffff;
static const unsigned int DT_OFFMESH_CON_BIDIR = 1;

static const int DT_MAX_AREAS = 64;

// Flags for addTile
enum dtTileFlags
{
	DT_TILE_FREE_DATA = 0x01,					// Navmesh owns the tile memory and should free it.
};

// Flags returned by findStraightPath().
enum dtStraightPathFlags
{
	DT_STRAIGHTPATH_START = 0x01,				// The vertex is the start position.
	DT_STRAIGHTPATH_END = 0x02,					// The vertex is the end position.
	DT_STRAIGHTPATH_OFFMESH_CONNECTION = 0x04,	// The vertex is start of an off-mesh link.
};

// Flags describing polygon properties.
enum dtPolyTypes
{
	DT_POLYTYPE_GROUND = 0,						// Regular ground polygons.
	DT_POLYTYPE_OFFMESH_CONNECTION = 1,			// Off-mesh connections.
};

// Structure describing the navigation polygon data.
struct dtPoly
{
	unsigned int firstLink;						// Index to first link in linked list. 
	unsigned short verts[DT_VERTS_PER_POLYGON];	// Indices to vertices of the poly.
	unsigned short neis[DT_VERTS_PER_POLYGON];	// Refs to neighbours of the poly.
	unsigned short flags;						// Flags (see dtPolyFlags).
	unsigned char vertCount;					// Number of vertices.
	unsigned char area : 6;						// Area ID of the polygon.
	unsigned char type : 2;						// Polygon type, see dtPolyTypes.
};

// Stucture describing polygon detail triangles.
struct dtPolyDetail
{
	unsigned short vertBase;					// Offset to detail vertex array.
	unsigned short vertCount;					// Number of vertices in the detail mesh.
	unsigned short triBase;						// Offset to detail triangle array.
	unsigned short triCount;					// Number of triangles.
};

// Stucture describing a link to another polygon.
struct dtLink
{
	dtPolyRef ref;							// Neighbour reference.
	unsigned int next;						// Index to next link.
	unsigned char edge;						// Index to polygon edge which owns this link. 
	unsigned char side;						// If boundary link, defines on which side the link is.
	unsigned char bmin, bmax;				// If boundary link, defines the sub edge area.
};

struct dtBVNode
{
	unsigned short bmin[3], bmax[3];		// BVnode bounds
	int i;									// Index to item or if negative, escape index.
};

struct dtOffMeshConnection
{
	float pos[6];							// Both end point locations.
	float rad;								// Link connection radius.
	unsigned short poly;					// Poly Id
	unsigned char flags;					// Link flags
	unsigned char side;						// End point side.
};

struct dtMeshHeader
{
	int magic;								// Magic number, used to identify the data.
	int version;							// Data version number.
	int x, y;								// Location of the time on the grid.
	unsigned int userId;					// User ID of the tile.
	int polyCount;							// Number of polygons in the tile.
	int vertCount;							// Number of vertices in the tile.
	int maxLinkCount;						// Number of allocated links.
	int detailMeshCount;					// Number of detail meshes.
	int detailVertCount;					// Number of detail vertices.
	int detailTriCount;						// Number of detail triangles.
	int bvNodeCount;						// Number of BVtree nodes.
	int offMeshConCount;					// Number of Off-Mesh links.
	int offMeshBase;						// Index to first polygon which is Off-Mesh link.
	float walkableHeight;					// Height of the agent.
	float walkableRadius;					// Radius of the agent
	float walkableClimb;					// Max climb height of the agent.
	float bmin[3], bmax[3];					// Bounding box of the tile.
	float bvQuantFactor;					// BVtree quantization factor (world to bvnode coords)
};

struct dtMeshTile
{
	unsigned int salt;						// Counter describing modifications to the tile.

	unsigned int linksFreeList;				// Index to next free link.
	dtMeshHeader* header;					// Pointer to tile header.
	dtPoly* polys;							// Pointer to the polygons (will be updated when tile is added).
	float* verts;							// Pointer to the vertices (will be updated when tile added).
	dtLink* links;							// Pointer to the links (will be updated when tile added).
	dtPolyDetail* detailMeshes;				// Pointer to detail meshes (will be updated when tile added).
	float* detailVerts;						// Pointer to detail vertices (will be updated when tile added).
	unsigned char* detailTris;				// Pointer to detail triangles (will be updated when tile added).
	dtBVNode* bvTree;						// Pointer to BVtree nodes (will be updated when tile added).
	dtOffMeshConnection* offMeshCons;		// Pointer to Off-Mesh links. (will be updated when tile added).
		
	unsigned char* data;					// Pointer to tile data.
	int dataSize;							// Size of the tile data.
	int flags;								// Tile flags, see dtTileFlags.
	dtMeshTile* next;						// Next free tile or, next tile in spatial grid.
};

struct dtNavMeshParams
{
	float orig[3];					// Origin of the nav mesh tile space.
	float tileWidth, tileHeight;	// Width and height of each tile.
	int maxTiles;					// Maximum number of tiles the navmesh can contain.
	int maxPolys;					// Maximum number of polygons each tile can contain.
};


class dtNavMesh
{
public:
	dtNavMesh();
	~dtNavMesh();

	// Initializes the nav mesh for tiled use.
	// Params:
	//  params - (in) navmesh initialization params, see dtNavMeshParams.
	// Returns: True if succeed, else false.
	bool init(const dtNavMeshParams* params);

	// Initializes the nav mesh for single tile use.
	// Params:
	//  data - (in) Data of the new tile mesh.
	//  dataSize - (in) Data size of the new tile mesh.
	//	flags - (in) Tile flags, see dtTileFlags.
	// Returns: True if succeed, else false.
	bool init(unsigned char* data, const int dataSize, const int flags);
	
	// Returns pointer to navmesh initialization params.
	const dtNavMeshParams* getParams() const;
	
	// Adds new tile into the navmesh.
	// The add will fail if the data is in wrong format,
	// there is not enough tiles left, or if there is a tile already at the location.
	// Params:
	//  data - (in) Data of the new tile mesh.
	//  dataSize - (in) Data size of the new tile mesh.
	//	flags - (in) Tile flags, see dtTileFlags.
	//  lastRef - (in,optional) Last tile ref, the tile will be restored so that
	//            the reference (as well as poly references) will be the same.  
	// Returns: Reference to the tile, 0 if failed. 
	dtTileRef addTile(unsigned char* data, int dataSize, int flags, dtTileRef lastRef = 0);
	
	// Removes specified tile.
	// Params:
	//  ref - (in) Reference to the tile to remove.
	//  data - (out) Data associated with deleted tile.
	//  dataSize - (out) Size of the data associated with deleted tile. 
	// Returns: True if remove suceed, else false.
	bool removeTile(dtTileRef ref, unsigned char** data, int* dataSize);

	// Calculates tile location based in input world position.
	// Params:
	//  pos - (in) world position of the query.
	//  tx - (out) tile x location.
	//  ty - (out) tile y location.
	void calcTileLoc(const float* pos, int* tx, int* ty);

	// Returns pointer to tile at specified location.
	// Params:
	//  x,y - (in) Location of the tile to get.
	// Returns: pointer to tile if tile exists or 0 tile does not exists.
	const dtMeshTile* getTileAt(int x, int y) const;

	// Returns reference to tile at specified location.
	// Params:
	//  x,y - (in) Location of the tile to get.
	// Returns: reference to tile if tile exists or 0 tile does not exists.
	dtTileRef getTileRefAt(int x, int y) const;
	
	// Returns tile references of a tile based on tile pointer.
	dtTileRef getTileRef(const dtMeshTile* tile) const;

	// Returns tile based on references.
	const dtMeshTile* getTileByRef(dtTileRef ref) const;
	
	// Returns max number of tiles.
	int getMaxTiles() const;
	
	// Returns pointer to tile in the tile array.
	// Params:
	//  i - (in) Index to the tile to retrieve, max index is getMaxTiles()-1.
	// Returns: Pointer to specified tile.
	const dtMeshTile* getTile(int i) const;

	// Returns pointer to tile and polygon pointed by the polygon reference.
	// Params:
	//  ref - (in) reference to a polygon.
	//  tile - (out) pointer to the tile containing the polygon.
	//  poly - (out) pointer to the polygon.
	// Returns false if poly ref is not valid, true on success.
	bool getTileAndPolyByRef(const dtPolyRef ref, const dtMeshTile** tile, const dtPoly** poly) const;
	
	// Returns pointer to tile and polygon pointed by the polygon reference.
	// Note: this function does not check if 'ref' s valid, and is thus faster. Use only with valid refs!
	// Params:
	//  ref - (in) reference to a polygon.
	//  tile - (out) pointer to the tile containing the polygon.
	//  poly - (out) pointer to the polygon.
	void getTileAndPolyByRefUnsafe(const dtPolyRef ref, const dtMeshTile** tile, const dtPoly** poly) const;

	// Returns true if polygon reference points to valid data.
	bool isValidPolyRef(dtPolyRef ref) const;
	
	// Returns base poly id for specified tile, polygon refs can be deducted from this.
	dtPolyRef getPolyRefBase(const dtMeshTile* tile) const;
	
	// Returns start and end location of an off-mesh link polygon.
	// Params:
	//	prevRef - (in) ref to the polygon before the link (used to select direction).
	//	polyRef - (in) ref to the off-mesh link polygon.
	//	startPos[3] - (out) start point of the link.
	//	endPos[3] - (out) end point of the link.
	// Returns: true if link is found.
	bool getOffMeshConnectionPolyEndPoints(dtPolyRef prevRef, dtPolyRef polyRef, float* startPos, float* endPos) const;
	
	// Sets polygon flags.
	void setPolyFlags(dtPolyRef ref, unsigned short flags);

	// Return polygon flags.
	unsigned short getPolyFlags(dtPolyRef ref) const;

	// Set polygon type.
	void setPolyArea(dtPolyRef ref, unsigned char area);

	// Return polygon area type.
	unsigned char getPolyArea(dtPolyRef ref) const;


	// Returns number of bytes required to store tile state.
	int getTileStateSize(const dtMeshTile* tile) const;
	
	// Stores tile state to buffer.
	bool storeTileState(const dtMeshTile* tile, unsigned char* data, const int maxDataSize) const;
	
	// Restores tile state.
	bool restoreTileState(dtMeshTile* tile, const unsigned char* data, const int maxDataSize);
	

	// Encodes a tile id.
	inline dtPolyRef encodePolyId(unsigned int salt, unsigned int it, unsigned int ip) const
	{
		return (salt << (m_polyBits+m_tileBits)) | (it << m_polyBits) | ip;
	}
	
	// Decodes a tile id.
	inline void decodePolyId(dtPolyRef ref, unsigned int& salt, unsigned int& it, unsigned int& ip) const
	{
		salt = (ref >> (m_polyBits+m_tileBits)) & ((1<<m_saltBits)-1);
		it = (ref >> m_polyBits) & ((1<<m_tileBits)-1);
		ip = ref & ((1<<m_polyBits)-1);
	}

	// Decodes a tile salt.
	inline unsigned int decodePolyIdSalt(dtPolyRef ref) const
	{
		return (ref >> (m_polyBits+m_tileBits)) & ((1<<m_saltBits)-1);
	}
	
	// Decodes a tile id.
	inline unsigned int decodePolyIdTile(dtPolyRef ref) const
	{
		return (ref >> m_polyBits) & ((1<<m_tileBits)-1);
	}
	
	// Decodes a poly id.
	inline unsigned int decodePolyIdPoly(dtPolyRef ref) const
	{
		return ref & ((1<<m_polyBits)-1);
	}
	
private:

	// Returns pointer to tile in the tile array.
	dtMeshTile* getTile(int i);

	// Returns neighbour tile based on side. 
	dtMeshTile* getNeighbourTileAt(int x, int y, int side) const;
	// Returns all polygons in neighbour tile based on portal defined by the segment.
	int findConnectingPolys(const float* va, const float* vb,
							const dtMeshTile* tile, int side,
							dtPolyRef* con, float* conarea, int maxcon) const;
	
	// Builds internal polygons links for a tile.
	void connectIntLinks(dtMeshTile* tile);
	// Builds internal polygons links for a tile.
	void connectIntOffMeshLinks(dtMeshTile* tile);

	// Builds external polygon links for a tile.
	void connectExtLinks(dtMeshTile* tile, dtMeshTile* target, int side);
	// Builds external polygon links for a tile.
	void connectExtOffMeshLinks(dtMeshTile* tile, dtMeshTile* target, int side);
	
	// Removes external links at specified side.
	void unconnectExtLinks(dtMeshTile* tile, int side);
	

	// TODO: These methods are duplicates from dtNavMeshQuery, but are needed for off-mesh connection finding.
	
	// Queries polygons within a tile.
	int queryPolygonsInTile(const dtMeshTile* tile, const float* qmin, const float* qmax,
							dtPolyRef* polys, const int maxPolys) const;
	// Find nearest polygon within a tile.
	dtPolyRef findNearestPolyInTile(const dtMeshTile* tile, const float* center,
									const float* extents, float* nearestPt) const;
	// Returns closest point on polygon.
	bool closestPointOnPolyInTile(const dtMeshTile* tile, unsigned int ip,
								  const float* pos, float* closest) const;
	
	dtNavMeshParams m_params;			// Current initialization params. TODO: do not store this info twice.
	float m_orig[3];					// Origin of the tile (0,0)
	float m_tileWidth, m_tileHeight;	// Dimensions of each tile.
	int m_maxTiles;						// Max number of tiles.
	int m_tileLutSize;					// Tile hash lookup size (must be pot).
	int m_tileLutMask;					// Tile hash lookup mask.

	dtMeshTile** m_posLookup;			// Tile hash lookup.
	dtMeshTile* m_nextFree;				// Freelist of tiles.
	dtMeshTile* m_tiles;				// List of tiles.
		
	unsigned int m_saltBits;			// Number of salt bits in the tile ID.
	unsigned int m_tileBits;			// Number of tile bits in the tile ID.
	unsigned int m_polyBits;			// Number of poly bits in the tile ID.
};

// Helper function to allocate navmesh class using Detour allocator.
dtNavMesh* dtAllocNavMesh();
void dtFreeNavMesh(dtNavMesh* navmesh);

#endif // DETOURNAVMESH_H
