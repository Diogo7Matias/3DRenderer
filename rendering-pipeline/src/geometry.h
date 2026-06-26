#pragma once

#include "math/vec3.h"
#include "vertex.h"
#include <vector>

namespace Geometry {

    //
    ////// Base class for primitives
    //
    class Primitive {
        std::vector<Vertex> _vertices;
        std::vector<std::pair<int,int>> _edges;

    public:
        virtual ~Primitive() = default;
    
        const std::vector<Vertex> &getVertices() const { return _vertices; }
        void setVertices(const std::vector<Vertex> &verts) { _vertices = verts; }
        const std::vector<std::pair<int,int>> &getEdges() const { return _edges; }
        void setEdges(const std::vector<std::pair<int,int>> &edges) { _edges = edges; }
    };

    // --------------------------------------------------------------------------

    class Cube : public Primitive {
        Vec3 _position;
        float _size;

    public:
        Cube(const Vec3 &position, float size) : _position(position), _size(size) {
            std::vector<Vertex> vertices;
            std::vector<std::pair<int,int>> edges;
            float h = size / 2.0f;

            // +X face
            buildFace(
                position + Vec3( h, -h, -h),
                position + Vec3( h,  h, -h),
                position + Vec3( h,  h,  h),
                position + Vec3( h, -h,  h),
                Vec3(1, 0, 0), vertices, edges
            );
            // -X face
            buildFace(
                position + Vec3(-h, -h,  h),
                position + Vec3(-h,  h,  h),
                position + Vec3(-h,  h, -h),
                position + Vec3(-h, -h, -h),
                Vec3(-1, 0, 0), vertices, edges
            );
            // +Y face
            buildFace(
                position + Vec3(-h,  h, -h),
                position + Vec3(-h,  h,  h),
                position + Vec3( h,  h,  h),
                position + Vec3( h,  h, -h),
                Vec3(0, 1, 0), vertices, edges
            );
            // -Y face
            buildFace(
                position + Vec3(-h, -h,  h),
                position + Vec3(-h, -h, -h),
                position + Vec3( h, -h, -h),
                position + Vec3( h, -h,  h),
                Vec3(0, -1, 0), vertices, edges
            );
            // +Z face
            buildFace(
                position + Vec3(-h, -h,  h),
                position + Vec3( h, -h,  h),
                position + Vec3( h,  h,  h),
                position + Vec3(-h,  h,  h),
                Vec3(0, 0, 1), vertices, edges
            );
            // -Z face
            buildFace(
                position + Vec3( h, -h, -h),
                position + Vec3(-h, -h, -h),
                position + Vec3(-h,  h, -h),
                position + Vec3( h,  h, -h),
                Vec3(0, 0, -1), vertices, edges
            );

            setVertices(vertices);
            setEdges(edges);
        }
        
        const Vec3 &position() const { return _position; }

        float size() const { return _size; }

    private:
        void buildFace(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 normal,
                    std::vector<Vertex>& vertices,
                    std::vector<std::pair<int,int>>& edges) {
            int base = vertices.size();
            vertices.push_back(Vertex(a, normal));
            vertices.push_back(Vertex(b, normal));
            vertices.push_back(Vertex(c, normal));
            vertices.push_back(Vertex(d, normal));

            edges.push_back({base + 0, base + 1});
            edges.push_back({base + 1, base + 2});
            edges.push_back({base + 2, base + 3});
            edges.push_back({base + 3, base + 0});
        }
    };

    // --------------------------------------------------------------------------

    class Sphere : public Primitive {
        Vec3 _center;
        float _radius;
        size_t _segments;

    public:
        Sphere(const Vec3 &center, float radius, size_t segments) : _center(center), _radius(radius), _segments(segments) {
            std::vector<Vertex> vertices;
            std::vector<std::pair<int,int>> edges;

            // Vertices
            for (size_t i = 0; i <= _segments; ++i) {
                float phi = (float)i / (float)_segments * M_PI;
                for (size_t j = 0; j <= _segments; ++j) {
                    float theta = (float)j / (float)_segments * 2 * M_PI;
                    Vec3 position = Vec3(
                        _center.x + _radius * std::sin(phi) * std::cos(theta),
                        _center.y + _radius * std::cos(phi),
                        _center.z + _radius * std::sin(phi) * std::sin(theta)
                    );
                    Vec3 normal = (position - center).normalize();
                    vertices.push_back(Vertex(position, normal));
                }
            }
            
            // Edges
            for (size_t i = 0; i < _segments; ++i) {
                for (size_t j = 0; j < _segments; ++j) {
                    int current = i * (_segments + 1) + j;
                    int next_col = i * (_segments + 1) + j + 1;
                    int next_row = (i + 1) * (_segments + 1) + j;

                    edges.push_back({current, next_col});
                    edges.push_back({current, next_row});
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