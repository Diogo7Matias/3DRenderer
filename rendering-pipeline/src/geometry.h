#pragma once

#include "math/vec3.h"
#include <vector>

namespace Geometry {

    class Cube {
        Vec3 _position;
        float _size;

        std::vector<Vec3> vertices;
        std::vector<std::pair<int,int>> edges;

    public:
        Cube(const Vec3 &position, float size) : _position(position), _size(size) {
            vertices = {
                _position + Vec3(_size / 2, _size / 2, _size / 2),
                _position + Vec3(_size / 2, _size / 2, -_size / 2),
                _position + Vec3(_size / 2, -_size / 2, _size / 2),
                _position + Vec3(_size / 2, -_size / 2, -_size / 2),
                _position + Vec3(-_size / 2, _size / 2, _size / 2),
                _position + Vec3(-_size / 2, _size / 2, -_size / 2),
                _position + Vec3(-_size / 2, -_size / 2, _size / 2),
                _position + Vec3(-_size / 2, -_size / 2, -_size / 2)
            };
            edges = {
                {0, 1}, {0, 2}, {0, 4},
                {1, 3}, {1, 5},
                {2, 3}, {2, 6},
                {3, 7},
                {4, 5}, {4, 6},
                {5, 7},
                {6, 7}
            };
        }
        
        const Vec3 &position() const { return _position; }

        float size() const { return _size; }

        const std::vector<Vec3> &getVertices() const { return vertices; }

        const std::vector<std::pair<int,int>> &getEdges() const { return edges; }
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