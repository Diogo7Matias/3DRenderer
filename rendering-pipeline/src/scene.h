#pragma once

#include "math/vec3.h"
#include "geometry.h"
#include "camera.h"

#include <vector>
#include <memory>

class Scene {
    std::vector<Vec3> _vertices;
    std::vector<std::pair<int,int>> _edges;
    
    // std::vector<Light> _lights;
    std::vector<std::unique_ptr<Camera>> _cameras;

public:
    Scene() {}
    
    std::vector<Vec3> vertices() const { return _vertices; }
    std::vector<std::pair<int,int>> edges() const { return _edges; }

    void add(Geometry::Primitive primitive);
    void add(std::unique_ptr<Camera> camera);
    
    Camera& getCamera(int index) const;
    int cameraCount() const { return _cameras.size(); }

private:
    void addVertex(Vec3 vertex) {
        _vertices.push_back(vertex);
    }

    void addEdge(int index1, int index2) {
        _edges.push_back(std::make_pair(index1, index2));
    }
};
