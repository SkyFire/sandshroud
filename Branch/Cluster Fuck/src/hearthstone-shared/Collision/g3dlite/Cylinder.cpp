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

#include "platform.h"
#include "Cylinder.h"
#include "LineSegment.h"
#include "CoordinateFrame.h"
#include "Line.h"
#include "AABox.h"

namespace G3D {

Cylinder::Cylinder() {
}


Cylinder::Cylinder(const Vector3& _p1, const Vector3& _p2, float _r) 
	: p1(_p1), p2(_p2), mRadius(_r) {
}


Line Cylinder::axis() const {
	return Line::fromTwoPoints(p1, p2);
}



float Cylinder::radius() const {
	return mRadius;
}


float Cylinder::volume() const {
	return
		(float)pi() * square(mRadius) * (p1 - p2).magnitude();
}


float Cylinder::area() const {
	return
		// Sides
		(twoPi() * mRadius) * height() +

		 // Caps
		 twoPi() * square(mRadius);
}

void Cylinder::getBounds(AABox& out) const {
	Vector3 min = p1.min(p2) - (Vector3(1, 1, 1) * mRadius);
	Vector3 max = p1.max(p2) + (Vector3(1, 1, 1) * mRadius);
	out = AABox(min, max);
}

bool Cylinder::contains(const Vector3& p) const { 
	return LineSegment::fromTwoPoints(p1, p2).distanceSquared(p) <= square(mRadius);
}


void Cylinder::getReferenceFrame(CoordinateFrame& cframe) const {
	cframe.translation = center();

	Vector3 Y = (p1 - p2).direction();
	Vector3 X = (abs(Y.dot(Vector3::unitX())) > 0.9) ? Vector3::unitY() : Vector3::unitX();
	Vector3 Z = X.cross(Y).direction();
	X = Y.cross(Z);		
	cframe.rotation.setColumn(0, X);
	cframe.rotation.setColumn(1, Y);
	cframe.rotation.setColumn(2, Z);
}


void Cylinder::getRandomSurfacePoint(Vector3& p, Vector3& N) const {
	float h = height();
	float r = radius();

	// Create a random point on a standard cylinder and then rotate to the global frame.

	// Relative areas (factor of 2PI already taken out)
	float capRelArea  = square(r) / 2.0f;
	float sideRelArea = r * h;

	float r1 = uniformRandom(0, capRelArea * 2 + sideRelArea);

	if (r1 < capRelArea * 2) {

		// Select a point uniformly at random on a disk
		// @cite http://mathworld.wolfram.com/DiskPointPicking.html
		float a = uniformRandom(0, (float)twoPi());
		float r2 = sqrt(uniformRandom(0, 1)) * r;
		p.x = cos(a) * r2;
		p.z = sin(a) * r2;

		N.x = 0;
		N.z = 0;
		if (r1 < capRelArea) {
			// Top
			p.y = h / 2.0f;
			N.y = 1;
		} else {
			// Bottom
			p.y = -h / 2.0f;
			N.y = -1;
		}
	} else {
		// Side
		float a = uniformRandom(0, (float)twoPi());
		N.x = cos(a);
		N.y = 0;
		N.z = sin(a);
		p.x = N.x * r;
		p.z = N.y * r;
		p.y = uniformRandom(-h / 2.0f, h / 2.0f);
	}

	// Transform to world space
	CoordinateFrame cframe;
	getReferenceFrame(cframe);
	
	p = cframe.pointToWorldSpace(p);
	N = cframe.normalToWorldSpace(N);
}


Vector3 Cylinder::randomInteriorPoint() const {
	float h = height();
	float r = radius();

	// Create a random point in a standard cylinder and then rotate to the global frame.

	// Select a point uniformly at random on a disk
	// @cite http://mathworld.wolfram.com/DiskPointPicking.html
	float a = uniformRandom(0, (float)twoPi());
	float r2 = sqrt(uniformRandom(0, 1)) * r;

	Vector3 p(  cos(a) * r2,
				uniformRandom(-h / 2.0f, h / 2.0f),
				sin(a) * r2);

	// Transform to world space
	CoordinateFrame cframe;
	getReferenceFrame(cframe);
	
	return cframe.pointToWorldSpace(p);
}

} // namespace
