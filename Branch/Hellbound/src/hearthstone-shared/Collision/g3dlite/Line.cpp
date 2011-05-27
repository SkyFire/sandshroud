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

#include "SharedStdAfx.h"
#include "Line.h"
#include "Plane.h"

namespace G3D {

Vector3 Line::intersection(const Plane& plane) const {
	float d;
	Vector3 normal = plane.normal();
	plane.getEquation(normal, d);
	float rate = _direction.dot(normal);

	if (rate == 0) {

		return Vector3::inf();

	} else {
		float t = -(d + _point.dot(normal)) / rate;

		return _point + _direction * t;
	}
}

Vector3 Line::closestPoint(const Vector3& pt) const {
	float t = _direction.dot(pt - _point);
	return _point + _direction * t;
}

Vector3 Line::point() const {
	return _point;
}


Vector3 Line::direction() const {
	return _direction;
}


Vector3 Line::closestPoint(const Line& B, float& minDist) const {
	const Vector3& P1 = _point;
	const Vector3& U1 = _direction;

	Vector3 P2 = B.point();
	Vector3 U2 = B.direction();
	
	const Vector3& P21 = P2 - P1;
	const Vector3& M = U2.cross(U1);
	float m2 = M.length();

	Vector3 R = P21.cross(M) / m2;
	
	float t1 = R.dot(U2);
	
	minDist = abs(P21.dot(M)) / sqrt(m2);

	return P1 + t1 * U1;
}

}

