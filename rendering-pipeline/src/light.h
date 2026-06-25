#pragma once

#include "color.h"

class Light {
    
protected:
    Color _color;
    float _intensity;

public:
    Light(Color color, float intensity) : _color(color), _intensity(intensity) {}
    
    Light(Color color) : _color(color) {
        _intensity = 1.0f;
    }

    Light() {
        _color = Color(0x000000);
        _intensity = 1.0f;
    }

    virtual ~Light() = default;

    virtual Color compute(Vec3 &v) const = 0;
};

// --------------------------------------------------------------------------

class AmbientLight : public Light {

public:
    AmbientLight(Color color, float intensity) : Light(color, intensity) {}
    AmbientLight(Color color) : Light(color) {}
    AmbientLight() : Light() {}

    Color compute(Vec3 &v) const override {
        return _color;
    }
};

// --------------------------------------------------------------------------

class PointLight : public Light {

public:
    PointLight(Color color, float intensity) : Light(color, intensity) {}
    PointLight(Color color) : Light(color) {}
    PointLight() : Light() {}

    Color compute(Vec3 &v) const override {
        return _color;
    }
};