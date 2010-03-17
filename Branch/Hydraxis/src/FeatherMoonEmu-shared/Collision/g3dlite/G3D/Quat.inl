/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

namespace G3D {

inline float& Quat::operator[] (int i) {
    debugAssert(i >= 0);
    debugAssert(i < 4);
    return ((float*)this)[i];
}

inline const float& Quat::operator[] (int i) const {
    debugAssert(i >= 0);
    debugAssert(i < 4);
    return ((float*)this)[i];
}



inline Quat Quat::operator-(const Quat& other) const {
    return Quat(x - other.x, y - other.y, z - other.z, w - other.w);
}

inline Quat Quat::operator+(const Quat& other) const {
    return Quat(x + other.x, y + other.y, z + other.z, w + other.w);
}

}

