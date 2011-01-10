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

#ifndef G3D_G3D_h
#define G3D_G3D_h

#include "platform.h"
#include "units.h"
#include "Random.h"
#include "Array.h"
#include "Crypto.h"
#include "format.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "CoordinateFrame.h"
#include "Plane.h"
#include "Line.h"
#include "Ray.h"
#include "Sphere.h"
#include "Box.h"
#include "AABox.h"
#include "Quat.h"
#include "stringutils.h"
#include "Table.h"
#include "Set.h"
#include "g3dfnmatch.h"
#include "G3DGameUnits.h"
#include "g3dmath.h"
#include "uint128.h"
#include "fileutils.h"
#include "ReferenceCount.h"
#include "MemoryManager.h"

template<class T> struct HashTrait< G3D::ReferenceCountedPointer<T> > {
	static size_t hashCode(G3D::ReferenceCountedPointer<T> key) { return reinterpret_cast<size_t>( key.pointer() ); }
};

#include "CollisionDetection.h"
#include "System.h"
#include "Spline.h"
#include "UprightFrame.h"
#include "LineSegment.h"
#include "Capsule.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "Vector2int16.h"
#include "Vector3int16.h"
#include "Vector3int32.h"
#include "Vector4int8.h"
#include "vectorMath.h"
#include "Rect2D.h"
#include "GCamera.h"
#include "AtomicInt32.h"
#include "RegistryUtil.h"
#include "filter.h"

#endif

