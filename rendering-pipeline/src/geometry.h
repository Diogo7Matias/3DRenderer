#pragma once

#include "math/vec3.h"
#include <vector>

namespace Geometry {

    //
    ////// Base class for primitives
    //
    class Primitive {
        std::vector<Vec3> vertices;
        std::vector<std::pair<int,int>> edges;

    public:
        virtual ~Primitive() = default;
    
        const std::vector<Vec3> &getVertices() const { return vertices; }
        void setVertices(const std::vector<Vec3> &verts) { vertices = verts; }
        const std::vector<std::pair<int,int>> &getEdges() const { return edges; }
        void setEdges(const std::vector<std::pair<int,int>> &edgs) { edges = edgs; }
    };

    // --------------------------------------------------------------------------

    class Cube : public Primitive {
        Vec3 _position;
        float _size;

    public:
        Cube(const Vec3 &position, float size) : _position(position), _size(size) {
            std::vector<Vec3> vertices = {
                _position + Vec3(_size / 2, _size / 2, _size / 2),
                _position + Vec3(_size / 2, _size / 2, -_size / 2),
                _position + Vec3(_size / 2, -_size / 2, _size / 2),
                _position + Vec3(_size / 2, -_size / 2, -_size / 2),
                _position + Vec3(-_size / 2, _size / 2, _size / 2),
                _position + Vec3(-_size / 2, _size / 2, -_size / 2),
                _position + Vec3(-_size / 2, -_size / 2, _size / 2),
                _position + Vec3(-_size / 2, -_size / 2, -_size / 2)
            };

            std::vector<std::pair<int,int>> edges = {
                {0, 1}, {0, 2}, {0, 4},
                {1, 3}, {1, 5},
                {2, 3}, {2, 6},
                {3, 7},
                {4, 5}, {4, 6},
                {5, 7},
                {6, 7}
            };

            setVertices(vertices);
            setEdges(edges);
        }
        
        const Vec3 &position() const { return _position; }

        float size() const { return _size; }
    };

    // --------------------------------------------------------------------------

    class Sphere : public Primitive {
        Vec3 _center;
        float _radius;
        size_t _segments;

    public:
        Sphere(const Vec3 &center, float radius, size_t segments) : _center(center), _radius(radius), _segments(segments) {
            std::vector<Vec3> vertices;
            std::vector<std::pair<int,int>> edges;

            for (size_t i = 0; i <= _segments; ++i) {
                float phi = (float)i / (float)_segments * M_PI;
                for (size_t j = 0; j <= _segments; ++j) {
                    float theta = (float)j / (float)_segments * 2 * M_PI;
                    Vec3 vertex = Vec3(
                        _center.x + _radius * std::sin(phi) * std::cos(theta),
                        _center.y + _radius * std::cos(phi),
                        _center.z + _radius * std::sin(phi) * std::sin(theta)
                    );
                    vertices.push_back(vertex);
                    // TODO: cool effect but edges are not correct. Will fix later
                    if (vertices.size() > 1 && j != 0) {
                        edges.push_back({vertices.size() - 2, vertices.size() - 1});
                    }
                }
            }

            setVertices(vertices);
            setEdges(edges);
        }

        Sphere(const Vec3 &center, float radius) : Sphere(center, radius, 16) {}

        const Vec3 &center() const { return _center; }
        
        float radius() const { return _radius; }
    };
}