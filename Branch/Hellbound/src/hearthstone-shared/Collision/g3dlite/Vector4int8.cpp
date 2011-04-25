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
#include "Vector4int8.h"
#include "Vector3.h"
#include "Vector4.h"
#include <string>

namespace G3D {

Vector4int8::Vector4int8(const Vector4& source) {
	x = iClamp(iRound(source.x), -128, 127);
	y = iClamp(iRound(source.y), -128, 127);
	z = iClamp(iRound(source.z), -128, 127);
	w = iClamp(iRound(source.w), -128, 127);
}

Vector4int8::Vector4int8(const Vector3& source, int8 w) : w(w) {
	x = iClamp(iRound(source.x), -128, 127);
	y = iClamp(iRound(source.y), -128, 127);
	z = iClamp(iRound(source.z), -128, 127);
}

} // namespace G3D

