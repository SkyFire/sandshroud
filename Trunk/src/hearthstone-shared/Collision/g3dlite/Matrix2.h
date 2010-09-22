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

#ifndef G3D_MATRIX2_H
#define G3D_MATRIX2_H

#include "platform.h"
#include "Vector2.h"

namespace G3D {

/** @beta */
class Matrix2 {
private:

	float		  data[2][2];

public:

	inline Matrix2() {
		data[0][0] = 1.0f; data[0][1] = 0.0f;
		data[1][0] = 0.0f; data[1][1] = 1.0f;
	}

	inline Matrix2(float v00, float v01, float v10, float v11) {
		data[0][0] = v00; data[0][1] = v01;
		data[1][0] = v10; data[1][1] = v11;
	}

	inline Vector2 operator*(const Vector2& v) const {
		return Vector2(data[0][0] * v[0] + data[0][1] * v[1],
					   data[1][0] * v[0] + data[1][1] * v[1]);
	}
	
	inline Matrix2 inverse() const {
		return Matrix2(data[0][0], data[1][0],
					   data[0][1], data[1][1]) * (1.0f / determinant());
	}

	inline Matrix2 transpose() const {
		return Matrix2(data[0][0], data[1][0],
					   data[0][1], data[1][1]);
	}

	inline float determinant() const {
		return data[0][0] * data[1][1] - data[0][1] * data[1][0];
	}

	inline Matrix2 operator*(float f) const {
		return Matrix2(data[0][0] * f, data[0][1] * f,
					   data[1][0] * f, data[1][1] * f);
	}

	inline Matrix2 operator/(float f) const {
		return Matrix2(data[0][0] / f, data[0][1] / f,
					   data[1][0] / f, data[1][1] / f);
	}

	inline float* operator[](int i) {
		debugAssert(i >= 0 && i <= 2);
		return data[i];
	}

	inline const float* operator[](int i) const {
		debugAssert(i >= 0 && i <= 1);
		return data[i];
	}
};

}

#endif
