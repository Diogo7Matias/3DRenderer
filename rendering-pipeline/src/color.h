#pragma once

#include <cstdint>
#include "math/vec3.h"

class Color {
    uint32_t _value;

public:
    Color() { _value = 0xffffff; }
    Color(uint32_t value) : _value(value) {}

    static Color fromVec3(const Vec3& v) {
        uint8_t r = (uint8_t)(v.x * 255.0f);
        uint8_t g = (uint8_t)(v.y * 255.0f);
        uint8_t b = (uint8_t)(v.z * 255.0f);
        return Color(0xFF000000 | (r << 16) | (g << 8) | b);
    }
    
    Vec3 asVec3() {
        float r = ((_value >> 16) & 0xFF) / 255.0f;
        float g = ((_value >>  8) & 0xFF) / 255.0f;
        float b = ((_value >>  0) & 0xFF) / 255.0f;
        return Vec3(r, g, b);
    }

    uint32_t value() const { return _value; }

    Color operator + (const Color &other) const { return _value + other.value(); }
    Color& operator += (const Color &other) { _value += other.value(); return *this; }

};