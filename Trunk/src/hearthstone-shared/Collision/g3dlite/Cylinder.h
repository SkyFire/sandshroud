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

#ifndef G3D_Cylinder_H
#define G3D_Cylinder_H

#include "platform.h"
#include "g3dmath.h"
#include "Vector3.h"

namespace G3D {

class Line;
class AABox;
/**
 Right cylinder
 */
class Cylinder {
private:
	Vector3			p1;
	Vector3			p2;

	float		   mRadius;

public:

	/** Uninitialized */
	Cylinder();
	Cylinder(const Vector3& _p1, const Vector3& _p2, float _r);
	
	/** The line down the center of the Cylinder */
	Line axis() const;

	/** 
	  A reference frame in which the center of mass is at the origin and
	  the Y-axis is the cylinder's axis.  If the cylinder is transformed, this reference frame
	  may freely rotate around its axis.*/
	void getReferenceFrame(class CoordinateFrame& cframe) const;

	/** Returns point 0 or 1 */
	inline const Vector3& point(int i) const {
		ASSERT(i >= 0 && i <= 1);
		return (i == 0) ? p1 : p2;
	}

	/**
	 Returns true if the point is inside the Cylinder or on its surface.
	 */
	bool contains(const Vector3& p) const;

	float area() const;

	float volume() const;

	float radius() const; 

	/** Center of mass */
	inline Vector3 center() const {
		return (p1 + p2) / 2.0f;
	}

	inline float height() const {
		return (p1 - p2).magnitude();
	}

	/**
	 Get close axis aligned bounding box.
	 With vertical world orientation, the top and bottom might not be very tight. */
	void getBounds(AABox& out) const;

	/** Random world space point with outward facing normal. */
	void getRandomSurfacePoint(Vector3& P, Vector3& N) const;

	/** Point selected uniformly at random over the volume. */
	Vector3 randomInteriorPoint() const;
};

} // namespace

#endif
