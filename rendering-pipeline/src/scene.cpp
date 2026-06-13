#include "scene.h"
#include <memory>

void Scene::add(Geometry::Cube cube) {
    for (Vec3 v : cube.getVertices()) {
        addVertex(v);
    }
    for (const auto& edge : cube.getEdges()) {
        addEdge(edge.first, edge.second);
    }
}

void Scene::add(Geometry::Sphere sphere) {
    // TODO
}

void Scene::add(std::unique_ptr<Camera> camera) {
    _cameras.push_back(std::move(camera));
}

const Camera& Scene::getCamera(int index) const {
    return *_cameras[index];
}
