/**
 @file Vector2int16.cpp
 
 @author Morgan McGuire, http://graphics.cs.williams.edu
  
 @created 2003-08-09
 @edited  2006-01-29
 */

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
