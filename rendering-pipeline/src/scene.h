#pragma once

#include "math/vec3.h"
#include "geometry.h"
#include "camera.h"

#include <vector>

class Scene {
    std::vector<Vec3> _vertices;
    // std::vector<Light> _lights;
    // std::vector<Camera> _cameras;

public:
    Scene() {}
    
    std::vector<Vec3> vertices() { return _vertices; }

    void add(Geometry::Cube cube);
    void add(Geometry::Sphere sphere);
    void add(Camera camera);

private:
    void addVertex(Vec3 vertex) {
        _vertices.push_back(vertex);
    }
};
