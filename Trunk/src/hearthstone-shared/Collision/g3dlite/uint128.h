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

#ifndef G3D_UINT128_H
#define G3D_UINT128_H

#include "g3dmath.h"

namespace G3D
{

	/** Limited functionality 128-bit unsigned integer. This is primarily to support FNV hashing and other
	cryptography applications.  See the GMP library for high-precision C++ math support. */
	class uint128
	{
	public:

		uint64 hi;
		uint64 lo;

		uint128(const uint64& lo);
		uint128(const uint64& hi, const uint64& lo);
		uint128& operator+=(const uint128& x);
		uint128& operator*=(const uint128& x);
		uint128& operator^=(const uint128& x);
		uint128& operator&=(const uint128& x);
		uint128& operator|=(const uint128& x);
		bool operator==(const uint128& x);
		uint128& operator>>=(const int x);
		uint128& operator<<=(const int x);
		uint128 operator&(const uint128& x);
	};
}

#endif
