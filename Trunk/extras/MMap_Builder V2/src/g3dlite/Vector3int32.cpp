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
#include "g3dmath.h"
#include "Vector3int32.h"
#include "Vector3int16.h"
#include "Vector3.h"
#include "format.h"

namespace G3D {

Vector3int32::Vector3int32(const class Vector3& v) {
	x = (int32)iFloor(v.x + 0.5);
	y = (int32)iFloor(v.y + 0.5);
	z = (int32)iFloor(v.z + 0.5);
}


Vector3int32::Vector3int32(const class Vector3int16& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

std::string Vector3int32::toString() const {
	return format("(%d, %d, %d)", x, y, z);
}

}
