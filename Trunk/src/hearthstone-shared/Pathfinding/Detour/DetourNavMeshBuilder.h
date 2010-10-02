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

#ifndef DETOURNAVMESHBUILDER_H
#define DETOURNAVMESHBUILDER_H

#include "DetourAlloc.h"


// The units of the parameters are specified in parenthesis as follows:
// (vx) voxels, (wu) world units
struct dtNavMeshCreateParams
{
	// Navmesh vertices.
	const unsigned short* verts;			// Array of vertices, each vertex has 3 components. (vx).
	int vertCount;							// Vertex count
	// Navmesh polygons
	const unsigned short* polys;			// Array of polygons, uses same format as rcPolyMesh. 
	const unsigned short* polyFlags;		// Array of flags per polygon.
	const unsigned char* polyAreas;			// Array of area ids per polygon.
	int polyCount;							// Number of polygons
	int nvp;								// Number of verts per polygon.
	// Navmesh Detail
	const unsigned short* detailMeshes;		// Detail meshes, uses same format as rcPolyMeshDetail.
	const float* detailVerts;				// Detail mesh vertices, uses same format as rcPolyMeshDetail (wu).
	int detailVertsCount;					// Total number of detail vertices
	const unsigned char* detailTris;		// Array of detail tris per detail mesh.
	int detailTriCount;						// Total number of detail triangles.
	// Off-Mesh Connections.
	const float* offMeshConVerts;			// Off-mesh connection vertices (wu).
	const float* offMeshConRad;				// Off-mesh connection radii (wu).
	const unsigned short* offMeshConFlags;	// Off-mesh connection flags.
	const unsigned char* offMeshConAreas;	// Off-mesh connection area ids.
	const unsigned char* offMeshConDir;		// Off-mesh connection direction flags (1 = bidir, 0 = oneway).
	int offMeshConCount;					// Number of off-mesh connections
	// Tile location
	unsigned int userId;					// User ID bound to the tile.
	int tileX, tileY;						// Tile location (tile coords).
	float bmin[3], bmax[3];					// Tile bounds (wu).
	// Settings
	float walkableHeight;					// Agent height (wu).
	float walkableRadius;					// Agent radius (wu).
	float walkableClimb;					// Agent max climb (wu).
	float cs;								// Cell size (xz) (wu).
	float ch;								// Cell height (y) (wu).
	int tileSize;							// Tile size (width & height) (vx).
};

// Build navmesh data from given input data.
bool dtCreateNavMeshData(dtNavMeshCreateParams* params, unsigned char** outData, int* outDataSize);

// Swaps endianess of navmesh header.
bool dtNavMeshHeaderSwapEndian(unsigned char* data, const int dataSize);

// Swaps endianess of the navmesh data. This function assumes that the header is in correct
// endianess already. Call dtNavMeshHeaderSwapEndian() first on the data if the data is
// assumed to be in wrong endianess to start with. If converting from native endianess to foreign,
// call dtNavMeshHeaderSwapEndian() after the data has been swapped.
bool dtNavMeshDataSwapEndian(unsigned char* data, const int dataSize);

#endif // DETOURNAVMESHBUILDER_H
