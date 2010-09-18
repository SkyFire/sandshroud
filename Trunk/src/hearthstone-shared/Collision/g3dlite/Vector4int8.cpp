/**
  @file Vector4int8.cpp
 
  Homogeneous vector class.

  @maintainer Morgan McGuire, http://graphics.cs.williams.edu
 
  @created 2007-02-09
  @edited  2007-02-09

  Copyright 2000-2007, Morgan McGuire.
  All rights reserved.
 */

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

