#include <iostream>
#include <vector>

#include "renderer.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "color.h"

void Renderer::render(const Scene &scene, const Camera &camera) {
    std::vector<Vec3> vertices = scene.vertices();
    std::vector<Light*> lights = scene.getLights();
    
    // clear fragment buffer
    memset(_fragmentBuffer, 0, _window->getWidth() * _window->getHeight() * sizeof(Fragment));

    for (Vec3 &v : vertices) {
        Vec4 vHomogeneous = v.toVec4();
        Mat4 view = camera.viewMatrix();
        Mat4 projection = camera.projectionMatrix();
        Mat4 viewport = _window->viewportMatrix();

        Vec3 vViewCoords = (view * vHomogeneous).toVec3();

        Color color;
        for (Light* light : lights) {
            color += light->compute(vViewCoords);
        }

        Vec3 vProjected = (projection * vViewCoords.toVec4()).toVec3();

        clipping(vProjected);

        v = (viewport * vProjected.toVec4()).toVec3();

        setFragmentColor(v, color);
    }
    
    for (const auto& edge : scene.edges()) {
        int x0 = (int)vertices[edge.first].x;
        int y0 = (int)vertices[edge.first].y;
        int x1 = (int)vertices[edge.second].x;
        int y1 = (int)vertices[edge.second].y;
        line(x0, y0, x1, y1);
    }
}

// Writes vertex to fragment buffer
void Renderer::setFragmentColor(Vec3 &v, Color &color) {
    int x = (int)v.x;
    int y = (int)v.y;
    
    if (x >= 0 && x < _window->getWidth() && y >= 0 && y < _window->getHeight()) {
        int index = y * _window->getWidth() + x;
        _fragmentBuffer[index].position = v;
        _fragmentBuffer[index].color = color.asVec3();
    }
}

// Cohen-Sutherland clipping algorithm
void Renderer::clipping(Vec3 &v) {
    // Define the clipping boundaries
    const float xMin = -1.0f, xMax = 1.0f;
    const float yMin = -1.0f, yMax = 1.0f;
    const float zMin = -1.0f, zMax = 1.0f;

    // Compute the outcode for the point
    int outcode = 0;
    if (v.x < xMin) outcode |= 1; // left
    else if (v.x > xMax) outcode |= 2; // right

    if (v.y < yMin) outcode |= 4; // bottom
    else if (v.y > yMax) outcode |= 8; // top
    
    if (v.z < zMin) outcode |= 16; // near
    else if (v.z > zMax) outcode |= 32; // far

    // If the point is outside the clipping volume, set it to the nearest boundary
    if (outcode != 0) {
        if (outcode & 1) v.x = xMin; // left
        else if (outcode & 2) v.x = xMax; // right

        if (outcode & 4) v.y = yMin; // bottom
        else if (outcode & 8) v.y = yMax; // top

        if (outcode & 16) v.z = zMin; // near
        else if (outcode & 32) v.z = zMax; // far
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
