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
        Mat4 view = camera.viewMatrix();
        Mat4 projection = camera.projectionMatrix();
        Mat4 viewport = _window->viewportMatrix();
        v = (viewport * projection *  view * vHomogeneous).toVec3();
        std::cout << "Viewport: " << v.x << ", " << v.y << ", " << v.z << std::endl;
    }

    // Write transformed vertices to fragment buffer
    for (Vec3 v : vertices) {
        int x = (int)v.x;
        int y = (int)v.y;
        
        if (x >= 0 && x < _window->getWidth() && y >= 0 && y < _window->getHeight()) {
            int index = y * _window->getWidth() + x;
            _fragmentBuffer[index].position = v;
            _fragmentBuffer[index].color = Vec3(1, 1, 1); // white
        }
    }
    
    for (const auto& edge : scene.edges()) {
        int x0 = (int)vertices[edge.first].x;
        int y0 = (int)vertices[edge.first].y;
        int x1 = (int)vertices[edge.second].x;
        int y1 = (int)vertices[edge.second].y;
        line(x0, y0, x1, y1);
    }
}

// this function implements the Bresenham's line drawing algorithm
void Renderer::line(int x0, int y0, int x1, int y1){
    // parameterization of the segment [A, B] where A = (x0, y0), B = (x1, y1):
    //      x(t) = x0 + t * (x1 - x0)
    //      y(t) = y0 + t * (y1 - y0)
    // t is a parameter between 0 and 1.

    if (x0 == x1 && y0 == y1) {
        _fragmentBuffer[y0 * _window->getWidth() + x0].color = Vec3(1, 1, 1); // white pixel
        return;
    }

    // if the line is steep (more "vertical" than "horizontal"), transpose the coordinates
    bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
    if (steep){
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    // if the starting point is to the right of the other end of the segment, swap endpoints
    if (x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // main component of the algorithm where we iterate over x to draw the line
    for (int x = x0; x <= x1; x++){
        float t = (x - x0) / (float)(x1 - x0);
        int y = std::round(y0 * (1. - t) + y1 * t);

        // check if we had previously transposed the coordinates and de-transpose if so
        if (steep){
            _fragmentBuffer[x * _window->getWidth() + y].color = Vec3(1, 1, 1); // white pixel
        } else {
            _fragmentBuffer[y * _window->getWidth() + x].color = Vec3(1, 1, 1); // white pixel
        }
    }
}
