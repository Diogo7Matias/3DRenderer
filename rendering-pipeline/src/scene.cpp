#include "scene.h"

void Scene::add(Geometry::Cube cube) {
    Vec3 vertices[8] = {
        cube.position() + Vec3(cube.size() / 2, cube.size() / 2, cube.size() / 2),
        cube.position() + Vec3(cube.size() / 2, cube.size() / 2, -cube.size() / 2),
        cube.position() + Vec3(cube.size() / 2, -cube.size() / 2, cube.size() / 2),
        cube.position() + Vec3(cube.size() / 2, -cube.size() / 2, -cube.size() / 2),
        cube.position() + Vec3(-cube.size() / 2, cube.size() / 2, cube.size() / 2),
        cube.position() + Vec3(-cube.size() / 2, cube.size() / 2, -cube.size() / 2),
        cube.position() + Vec3(-cube.size() / 2, -cube.size() / 2, cube.size() / 2),
        cube.position() + Vec3(-cube.size() / 2, -cube.size() / 2, -cube.size() / 2)
    };

    for (Vec3 v : vertices) {
        addVertex(v);
    }
}

void Scene::add(Geometry::Sphere sphere) {
    // TODO
}

void Scene::add(Camera camera) {
    _cameras.push_back(camera);
}
