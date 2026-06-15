#pragma once

#include "vec3.h"

struct Vec4 {
    
    float x, y, z, w;

    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vec3 toVec3() const;

    ///
    ////// * Operator Overloads
    ///

    Vec4 operator+(const Vec4& other) const { return {x + other.x, y + other.y, z + other.z, w + other.w}; }
    Vec4 operator-(const Vec4& other) const { return {x - other.x, y - other.y, z - other.z, w - other.w}; }
    Vec4 operator*(float t) const { return {x * t, y * t, z * t, w * t}; }
    Vec4 operator- () const { return Vec4(-x, -y, -z, -w); }

    ///
    ////// * Methods
    ///

    float dot(const Vec4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }
};

inline Vec3 Vec4::toVec3() const {
    return Vec3(x/w, y/w, z/w);
}

inline Vec4 Vec3::toVec4(float w) const {
    return Vec4(x, y, z, w);
}
