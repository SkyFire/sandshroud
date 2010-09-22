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

#ifndef G3D_UPRIGHTFRAME_H
#define G3D_UPRIGHTFRAME_H

#include "platform.h"
#include "Spline.h"
#include "Vector3.h"
#include "CoordinateFrame.h"

namespace G3D {

	/**
	Coordinate frame expressed in Euler angles.
	Unlike a G3D::Quat, UprightFrame always keeps the reference frame from rolling about its own z axis.
	Particularly useful for cameras.

	@sa G3D::CoordinateFrame, G3D::Matrix4, G3D::PhysicsFrame, G3D::UprightSpline, G3D::UprightSplineManipulator
	*/
	class UprightFrame
	{
	public:

		Vector3		 translation;

		/** -pi/2 < pitch < pi/2 in radians about the X-axis */
		float pitch;

		/** In radians about the Y-axis */
		float yaw;

		inline UprightFrame(const Vector3& t = Vector3::zero(), float p = 0, float y = 0)
			: translation(t), pitch(p), yaw(y) {}

		UprightFrame(const CoordinateFrame& cframe);
		CoordinateFrame toCoordinateFrame() const;

		/** Supports implicit cast to CoordinateFrame */
		inline operator CoordinateFrame() const { return toCoordinateFrame(); }

		/** Required for use with spline */
		UprightFrame operator+(const UprightFrame& other) const;

		/** Required for use with spline */
		UprightFrame operator*(const float k) const;

		/**	Unwraps the yaw values in the elements of the array such that
		they still represent the same angles but strictly increase/decrease
		without wrapping about zero.  For use with Spline<UprightFrame> */
		static void unwrapYaw(UprightFrame* a, int N);
	};

	/** Shortest-path linear velocity spline for camera positions.  Always keeps the camera from rolling.
	@sa G3D::UprightSplineManipulator, G3D::UprightFrame */
	class UprightSpline : public Spline<UprightFrame>
	{
	protected:
		virtual void ensureShortestPath(UprightFrame* A, int N) const { UprightFrame::unwrapYaw(A, N); }
	};
}

#endif
