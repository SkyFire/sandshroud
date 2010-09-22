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

#include "SharedStdAfx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "Recast.h"
#include "RecastTimer.h"
#include "RecastLog.h"

inline bool overlapBounds(const float* amin, const float* amax, const float* bmin, const float* bmax)
{
	bool overlap = true;
	overlap = (amin[0] > bmax[0] || amax[0] < bmin[0]) ? false : overlap;
	overlap = (amin[1] > bmax[1] || amax[1] < bmin[1]) ? false : overlap;
	overlap = (amin[2] > bmax[2] || amax[2] < bmin[2]) ? false : overlap;
	return overlap;
}

inline bool overlapInterval(unsigned short amin, unsigned short amax,
							unsigned short bmin, unsigned short bmax)
{
	if (amax < bmin) return false;
	if (amin > bmax) return false;
	return true;
}


static rcSpan* allocSpan(rcHeightfield& hf)
{
	// If running out of memory, allocate new page and update the freelist.
	if (!hf.freelist || !hf.freelist->next)
	{
		// Create new page.
		// Allocate memory for the new pool.
		const int size = (sizeof(rcSpanPool)-sizeof(rcSpan)) + sizeof(rcSpan)*RC_SPANS_PER_POOL;
		rcSpanPool* pool = reinterpret_cast<rcSpanPool*>(new unsigned char[size]);
		if (!pool) return 0;
		pool->next = 0;
		// Add the pool into the list of pools.
		pool->next = hf.pools;
		hf.pools = pool;
		// Add new items to the free list.
		rcSpan* freelist = hf.freelist;
		rcSpan* head = &pool->items[0];
		rcSpan* it = &pool->items[RC_SPANS_PER_POOL];
		do
		{
			--it;
			it->next = freelist;
			freelist = it;
		}
		while (it != head);
		hf.freelist = it;
	}
	
	// Pop item from in front of the free list.
	rcSpan* it = hf.freelist;
	hf.freelist = hf.freelist->next;
	return it;
}

static void freeSpan(rcHeightfield& hf, rcSpan* ptr)
{
	if (!ptr) return;
	// Add the node in front of the free list.
	ptr->next = hf.freelist;
	hf.freelist = ptr;
}

void rcAddSpan(rcHeightfield& hf, const int x, const int y,
			   const unsigned short smin, const unsigned short smax,
			   const unsigned short flags, const int flagMergeThr)
{
	int idx = x + y*hf.width;
	
	rcSpan* s = allocSpan(hf);
	s->smin = smin;
	s->smax = smax;
	s->flags = flags;
	s->next = 0;
	
	// Empty cell, add he first span.
	if (!hf.spans[idx])
	{
		hf.spans[idx] = s;
		return;
	}
	rcSpan* prev = 0;
	rcSpan* cur = hf.spans[idx];
	
	// Insert and merge spans.
	while (cur)
	{
		if (cur->smin > s->smax)
		{
			// Current span is further than the new span, break.
			break;
		}
		else if (cur->smax < s->smin)
		{
			// Current span is before the new span advance.
			prev = cur;
			cur = cur->next;
		}
		else
		{
			// Merge spans.
			if (cur->smin < s->smin)
				s->smin = cur->smin;
			if (cur->smax > s->smax)
				s->smax = cur->smax;
			
			// Merge flags.
			if (rcAbs((int)s->smax - (int)cur->smax) <= flagMergeThr)
				s->flags |= cur->flags;
			
			// Remove current span.
			rcSpan* next = cur->next;
			freeSpan(hf, cur);
			if (prev)
				prev->next = next;
			else
				hf.spans[idx] = next;
			cur = next;
		}
	}
	
	// Insert new span.
	if (prev)
	{
		s->next = prev->next;
		prev->next = s;
	}
	else
	{
		s->next = hf.spans[idx];
		hf.spans[idx] = s;
	}
}

static int clipPoly(const float* in, int n, float* out, float pnx, float pnz, float pd)
{
	float d[12];
	for (int i = 0; i < n; ++i)
		d[i] = pnx*in[i*3+0] + pnz*in[i*3+2] + pd;
	
	int m = 0;
	for (int i = 0, j = n-1; i < n; j=i, ++i)
	{
		bool ina = d[j] >= 0;
		bool inb = d[i] >= 0;
		if (ina != inb)
		{
			float s = d[j] / (d[j] - d[i]);
			out[m*3+0] = in[j*3+0] + (in[i*3+0] - in[j*3+0])*s;
			out[m*3+1] = in[j*3+1] + (in[i*3+1] - in[j*3+1])*s;
			out[m*3+2] = in[j*3+2] + (in[i*3+2] - in[j*3+2])*s;
			m++;
		}
		if (inb)
		{
			out[m*3+0] = in[i*3+0];
			out[m*3+1] = in[i*3+1];
			out[m*3+2] = in[i*3+2];
			m++;
		}
	}
	return m;
}

static void rasterizeTri(const float* v0, const float* v1, const float* v2,
						 unsigned char flags, rcHeightfield& hf,
						 const float* bmin, const float* bmax,
						 const float cs, const float ics, const float ich,
						 const int flagMergeThr)
{
	const int w = hf.width;
	const int h = hf.height;
	float tmin[3], tmax[3];
	const float by = bmax[1] - bmin[1];
	
	// Calculate the bounding box of the triangle.
	vcopy(tmin, v0);
	vcopy(tmax, v0);
	vmin(tmin, v1);
	vmin(tmin, v2);
	vmax(tmax, v1);
	vmax(tmax, v2);
	
	// If the triangle does not touch the bbox of the heightfield, skip the triagle.
	if (!overlapBounds(bmin, bmax, tmin, tmax))
		return;
	
	// Calculate the footpring of the triangle on the grid.
	int x0 = (int)((tmin[0] - bmin[0])*ics);
	int y0 = (int)((tmin[2] - bmin[2])*ics);
	int x1 = (int)((tmax[0] - bmin[0])*ics);
	int y1 = (int)((tmax[2] - bmin[2])*ics);
	x0 = rcClamp(x0, 0, w-1);
	y0 = rcClamp(y0, 0, h-1);
	x1 = rcClamp(x1, 0, w-1);
	y1 = rcClamp(y1, 0, h-1);
	
	// Clip the triangle into all grid cells it touches.
	float in[7*3], out[7*3], inrow[7*3];
	
	for (int y = y0; y <= y1; ++y)
	{
		// Clip polygon to row.
		vcopy(&in[0], v0);
		vcopy(&in[1*3], v1);
		vcopy(&in[2*3], v2);
		int nvrow = 3;
		const float cz = bmin[2] + y*cs;
		nvrow = clipPoly(in, nvrow, out, 0, 1, -cz);
		if (nvrow < 3) continue;
		nvrow = clipPoly(out, nvrow, inrow, 0, -1, cz+cs);
		if (nvrow < 3) continue;
		
		for (int x = x0; x <= x1; ++x)
		{
			// Clip polygon to column.
			int nv = nvrow;
			const float cx = bmin[0] + x*cs;
			nv = clipPoly(inrow, nv, out, 1, 0, -cx);
			if (nv < 3) continue;
			nv = clipPoly(out, nv, in, -1, 0, cx+cs);
			if (nv < 3) continue;
			
			// Calculate min and max of the span.
			float smin = in[1], smax = in[1];
			for (int i = 1; i < nv; ++i)
			{
				smin = rcMin(smin, in[i*3+1]);
				smax = rcMax(smax, in[i*3+1]);
			}
			smin -= bmin[1];
			smax -= bmin[1];
			// Skip the span if it is outside the heightfield bbox
			if (smax < 0.0f) continue;
			if (smin > by) continue;
			// Clamp the span to the heightfield bbox.
			if (smin < 0.0f) smin = 0;
			if (smax > by) smax = by;
			
			// Snap the span to the heightfield height grid.
			unsigned short ismin = (unsigned short)rcClamp((int)floorf(smin * ich), 0, 0x7fff);
			unsigned short ismax = (unsigned short)rcClamp((int)ceilf(smax * ich), 0, 0x7fff);
			
			rcAddSpan(hf, x, y, ismin, ismax, flags, flagMergeThr);
		}
	}
}

void rcRasterizeTriangle(const float* v0, const float* v1, const float* v2,
						 unsigned char flags, rcHeightfield& solid,
						 const int flagMergeThr)
{
	rcTimeVal startTime = rcGetPerformanceTimer();

	const float ics = 1.0f/solid.cs;
	const float ich = 1.0f/solid.ch;
	rasterizeTri(v0, v1, v2, flags, solid, solid.bmin, solid.bmax, solid.cs, ics, ich, flagMergeThr);

	rcTimeVal endTime = rcGetPerformanceTimer();
	
	if (rcGetBuildTimes())
		rcGetBuildTimes()->rasterizeTriangles += rcGetDeltaTimeUsec(startTime, endTime);
}

void rcRasterizeTriangles(const float* verts, int nv,
						  const int* tris, const unsigned char* flags, int nt,
						  rcHeightfield& solid, const int flagMergeThr)
{
	rcTimeVal startTime = rcGetPerformanceTimer();
	
	const float ics = 1.0f/solid.cs;
	const float ich = 1.0f/solid.ch;
	// Rasterize triangles.
	for (int i = 0; i < nt; ++i)
	{
		const float* v0 = &verts[tris[i*3+0]*3];
		const float* v1 = &verts[tris[i*3+1]*3];
		const float* v2 = &verts[tris[i*3+2]*3];
		// Rasterize.
		rasterizeTri(v0, v1, v2, flags[i], solid, solid.bmin, solid.bmax, solid.cs, ics, ich, flagMergeThr);
	}
	
	rcTimeVal endTime = rcGetPerformanceTimer();

	if (rcGetBuildTimes())
		rcGetBuildTimes()->rasterizeTriangles += rcGetDeltaTimeUsec(startTime, endTime);
}

void rcRasterizeTriangles(const float* verts, int nv,
						  const unsigned short* tris, const unsigned char* flags, int nt,
						  rcHeightfield& solid, const int flagMergeThr)
{
	rcTimeVal startTime = rcGetPerformanceTimer();
	
	const float ics = 1.0f/solid.cs;
	const float ich = 1.0f/solid.ch;
	// Rasterize triangles.
	for (int i = 0; i < nt; ++i)
	{
		const float* v0 = &verts[tris[i*3+0]*3];
		const float* v1 = &verts[tris[i*3+1]*3];
		const float* v2 = &verts[tris[i*3+2]*3];
		// Rasterize.
		rasterizeTri(v0, v1, v2, flags[i], solid, solid.bmin, solid.bmax, solid.cs, ics, ich, flagMergeThr);
	}
	
	rcTimeVal endTime = rcGetPerformanceTimer();
	
	if (rcGetBuildTimes())
		rcGetBuildTimes()->rasterizeTriangles += rcGetDeltaTimeUsec(startTime, endTime);
}

void rcRasterizeTriangles(const float* verts, const unsigned char* flags, int nt,
						  rcHeightfield& solid, const int flagMergeThr)
{
	rcTimeVal startTime = rcGetPerformanceTimer();
	
	const float ics = 1.0f/solid.cs;
	const float ich = 1.0f/solid.ch;
	// Rasterize triangles.
	for (int i = 0; i < nt; ++i)
	{
		const float* v0 = &verts[(i*3+0)*3];
		const float* v1 = &verts[(i*3+1)*3];
		const float* v2 = &verts[(i*3+2)*3];
		// Rasterize.
		rasterizeTri(v0, v1, v2, flags[i], solid, solid.bmin, solid.bmax, solid.cs, ics, ich, flagMergeThr);
	}
	
	rcTimeVal endTime = rcGetPerformanceTimer();
	
	if (rcGetBuildTimes())
		rcGetBuildTimes()->rasterizeTriangles += rcGetDeltaTimeUsec(startTime, endTime);
}
