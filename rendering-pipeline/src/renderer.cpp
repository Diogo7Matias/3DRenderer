#include <iostream>
#include <vector>

#include "renderer.h"
#include "math/vec4.h"
#include "math/mat4.h"

void Renderer::render(const Scene &scene, const Camera &camera) {
    std::vector<Vec3> vertices = scene.vertices();

    // debug
    for (Vec3 v : vertices) {
        std::cout << "Vertex: " << v.x << ", " << v.y << ", " << v.z << std::endl;
    }

    // debug
    std::cout << "Camera Position: " << camera.position().x << ", " << camera.position().y << ", " << camera.position().z << std::endl;

    for (Vec3 &v : vertices) {
        Vec4 vHomogeneous = v.toVec4();
        v = toCameraCoordinates(vHomogeneous, camera).toVec3();
    }

    // debug
    for (Vec3 v : vertices) {
        std::cout << "Vertex: " << v.x << ", " << v.y << ", " << v.z << std::endl;
    }
}

Vec4 Renderer::toCameraCoordinates(Vec4 vertex, const Camera &camera) {
    Vec3 v = camera.up();
    Vec3 n = camera.direction();
    Vec3 u = n.cross(v).normalize();

    Mat4 translation = Mat4::translation(-camera.position());
    Mat4 rotation = Mat4::fromBasis(u, v, -n);
    Mat4 scale = Mat4::scale(Vec3(1, 1, -1));
    Mat4 viewMatrix = scale * rotation * translation;
    
    return viewMatrix * vertex;
}


