#pragma once

#include "math/vec3.h"

namespace Geometry {

    class Cube {
        Vec3 _position;
        float _size;

    public:
        Cube(const Vec3 &position, float size) : _position(position), _size(size) {}
        
        const Vec3 &position() const { return _position; }

        float size() const { return _size; }    
    };

    class Sphere {
        Vec3 _center;
        float _radius;

    public:
        Sphere(const Vec3 &center, float radius) : _center(center), _radius(radius) {}
    
        const Vec3 &center() const { return _center; }
        
        float radius() const { return _radius; }
    };
}