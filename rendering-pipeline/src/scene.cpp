#include "scene.h"
#include <memory>

void Scene::add(Geometry::Primitive primitive) {
    for (Vec3 v : primitive.getVertices()) {
        addVertex(v);
    }
    for (const auto& edge : primitive.getEdges()) {
        addEdge(edge.first, edge.second);
    }
}

void Scene::add(std::unique_ptr<Camera> camera) {
    _cameras.push_back(std::move(camera));
}

Camera& Scene::getCamera(int index) const {
    return *_cameras[index];
}
