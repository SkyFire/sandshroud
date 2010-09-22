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
#include <math.h>
#include "DetourCommon.h"

void closestPtPointTriangle(float* closest, const float* p,
							const float* a, const float* b, const float* c)
{
	// Check if P in vertex region outside A
	float ab[3], ac[3], ap[3];
	vsub(ab, b, a);
	vsub(ac, c, a);
	vsub(ap, p, a);
	float d1 = vdot(ab, ap);
	float d2 = vdot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// barycentric coordinates (1,0,0)
		vcopy(closest, a);
		return;
	}
	
	// Check if P in vertex region outside B
	float bp[3];
	vsub(bp, p, b);
	float d3 = vdot(ab, bp);
	float d4 = vdot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3)
	{
		// barycentric coordinates (0,1,0)
		vcopy(closest, b);
		return;
	}
	
	// Check if P in edge region of AB, if so return projection of P onto AB
	float vc = d1*d4 - d3*d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		// barycentric coordinates (1-v,v,0)
		float v = d1 / (d1 - d3);
		closest[0] = a[0] + v * ab[0];
		closest[1] = a[1] + v * ab[1];
		closest[2] = a[2] + v * ab[2];
		return;
	}
	
	// Check if P in vertex region outside C
	float cp[3];
	vsub(cp, p, c);
	float d5 = vdot(ab, cp);
	float d6 = vdot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6)
	{
		// barycentric coordinates (0,0,1)
		vcopy(closest, c);
		return;
	}
	
	// Check if P in edge region of AC, if so return projection of P onto AC
	float vb = d5*d2 - d1*d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		// barycentric coordinates (1-w,0,w)
		float w = d2 / (d2 - d6);
		closest[0] = a[0] + w * ac[0];
		closest[1] = a[1] + w * ac[1];
		closest[2] = a[2] + w * ac[2];
		return;
	}
	
	// Check if P in edge region of BC, if so return projection of P onto BC
	float va = d3*d6 - d5*d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		// barycentric coordinates (0,1-w,w)
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		closest[0] = b[0] + w * (c[0] - b[0]);
		closest[1] = b[1] + w * (c[1] - b[1]);
		closest[2] = b[2] + w * (c[2] - b[2]);
		return;
	}
	
	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	closest[0] = a[0] + ab[0] * v + ac[0] * w;
	closest[1] = a[1] + ab[1] * v + ac[1] * w;
	closest[2] = a[2] + ab[2] * v + ac[2] * w;
}

bool intersectSegmentPoly2D(const float* p0, const float* p1,
							const float* verts, int nverts,
							float& tmin, float& tmax,
							int& segMin, int& segMax)
{
	static const float EPS = 0.00000001f;
	
	tmin = 0;
	tmax = 1;
	segMin = -1;
	segMax = -1;
	
	float dir[3];
	vsub(dir, p1, p0);
	
	for (int i = 0, j = nverts-1; i < nverts; j=i++)
	{
		float edge[3], diff[3];
		vsub(edge, &verts[i*3], &verts[j*3]);
		vsub(diff, p0, &verts[j*3]);
		float n = vperp2D(edge, diff);
		float d = -vperp2D(edge, dir);
		if (fabs(d) < EPS)
		{
			// S is nearly parallel to this edge
			if (n < 0)
				return false;
			else
				continue;
		}
		float t = n / d;
		if (d < 0)
		{
			// segment S is entering across this edge
			if (t > tmin)
			{
				tmin = t;
				segMin = j;
				// S enters after leaving polygon
				if (tmin > tmax)
					return false;
			}
		}
		else
		{
			// segment S is leaving across this edge
			if (t < tmax)
			{
				tmax = t;
				segMax = j;
				// S leaves before entering polygon
				if (tmax < tmin)
					return false;
			}
		}
	}
	
	return true;
}

float distancePtSegSqr2D(const float* pt, const float* p, const float* q, float& t)
{
	float pqx = q[0] - p[0];
	float pqz = q[2] - p[2];
	float dx = pt[0] - p[0];
	float dz = pt[2] - p[2];
	float d = pqx*pqx + pqz*pqz;
	t = pqx*dx + pqz*dz;
	if (d > 0) t /= d;
	if (t < 0) t = 0;
	else if (t > 1) t = 1;
	dx = p[0] + t*pqx - pt[0];
	dz = p[2] + t*pqz - pt[2];
	return dx*dx + dz*dz;
}

void calcPolyCenter(float* tc, const unsigned short* idx, int nidx, const float* verts)
{
	tc[0] = 0.0f;
	tc[1] = 0.0f;
	tc[2] = 0.0f;
	for (int j = 0; j < nidx; ++j)
	{
		const float* v = &verts[idx[j]*3];
		tc[0] += v[0];
		tc[1] += v[1];
		tc[2] += v[2];
	}
	const float s = 1.0f / nidx;
	tc[0] *= s;
	tc[1] *= s;
	tc[2] *= s;
}

bool closestHeightPointTriangle(const float* p, const float* a, const float* b, const float* c, float& h)
{
	float v0[3], v1[3], v2[3];
	vsub(v0, c,a);
	vsub(v1, b,a);
	vsub(v2, p,a);
	
	const float dot00 = vdot2D(v0, v0);
	const float dot01 = vdot2D(v0, v1);
	const float dot02 = vdot2D(v0, v2);
	const float dot11 = vdot2D(v1, v1);
	const float dot12 = vdot2D(v1, v2);
	
	// Compute barycentric coordinates
	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// The (sloppy) epsilon is needed to allow to get height of points which
	// are interpolated along the edges of the triangles.
	static const float EPS = 1e-4f;
	
	// If point lies inside the triangle, return interpolated ycoord.
	if (u >= -EPS && v >= -EPS && (u+v) <= 1+EPS)
	{
		h = a[1] + v0[1]*u + v1[1]*v;
		return true;
	}
	
	return false;
}

bool distancePtPolyEdgesSqr(const float* pt, const float* verts, const int nverts,
							float* ed, float* et)
{
	// TODO: Replace pnpoly with triArea2D tests?
	int i, j;
	bool c = false;
	for (i = 0, j = nverts-1; i < nverts; j = i++)
	{
		const float* vi = &verts[i*3];
		const float* vj = &verts[j*3];
		if (((vi[2] > pt[2]) != (vj[2] > pt[2])) &&
			(pt[0] < (vj[0]-vi[0]) * (pt[2]-vi[2]) / (vj[2]-vi[2]) + vi[0]) )
			c = !c;
		ed[j] = distancePtSegSqr2D(pt, vj, vi, et[j]);
	}
	return c;
}
