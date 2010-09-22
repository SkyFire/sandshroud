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
#include "platform.h"
#include "g3dmath.h"
#include "Vector2int16.h"
#include "Vector2.h"

namespace G3D {

Vector2int16::Vector2int16(const class Vector2& v) {
	x = (int16)iFloor(v.x + 0.5);
	y = (int16)iFloor(v.y + 0.5);
}

Vector2int16 Vector2int16::clamp(const Vector2int16& lo, const Vector2int16& hi) {
	return Vector2int16(iClamp(x, lo.x, hi.x), iClamp(y, lo.y, hi.y));
}


}
