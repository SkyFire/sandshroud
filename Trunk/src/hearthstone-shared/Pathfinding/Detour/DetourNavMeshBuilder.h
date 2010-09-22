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

struct dtNavMeshCreateParams
{
	// Navmesh vertices.
	const unsigned short* verts;
	int vertCount;
	// Navmesh polygons
	const unsigned short* polys;
	const unsigned short* polyFlags;
	const unsigned char* polyAreas;
	int polyCount;
	int nvp;
	// Navmesh Detail
	const unsigned short* detailMeshes;
	const float* detailVerts;
	int detailVertsCount;
	const unsigned char* detailTris;
	int detailTriCount; 
	// Off-Mesh Connections.
	const float* offMeshConVerts;
	const float* offMeshConRad;
	const unsigned short* offMeshConFlags;
	const unsigned char* offMeshConAreas;
	const unsigned char* offMeshConDir;
	int offMeshConCount;
	// Settings
	float walkableHeight;
	float walkableRadius;
	float walkableClimb;
	float bmin[3], bmax[3];
	float cs;
	float ch;
	int tileSize;
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
