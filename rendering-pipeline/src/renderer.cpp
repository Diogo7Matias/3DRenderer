#include <iostream>
#include <vector>

#include "renderer.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "color.h"

void Renderer::render(const Scene &scene, const Camera &camera) {
    std::vector<Vertex> vertices = scene.vertices();
    std::vector<Material> materials = scene.materials();
    std::vector<Light*> lights = scene.getLights();
    
    // clear fragment buffer
    memset(_fragmentBuffer, 0, _window->getWidth() * _window->getHeight() * sizeof(Fragment));

    Mat4 view = camera.viewMatrix();
    Mat4 projection = camera.projectionMatrix();
    Mat4 viewport = _window->viewportMatrix();
    Mat4 normalMatrix = view.inverseTranspose();

    for (Light* light : lights) {
        if (PointLight* pl = dynamic_cast<PointLight*>(light)) {
            pl->setViewPosition(view);
        }
    }
    
    // transform to view space (camera coordinates)
    for (Vertex &v : vertices) {
        Vec3 &pos = v.position;

        pos = (view * pos.toVec4()).toVec3();
        Vec4 n = normalMatrix * Vec4(v.normal.x, v.normal.y, v.normal.z, 0.0f);
        v.normal = Vec3(n.x, n.y, n.z).normalize();
    }

    for (const Triangle& t : scene.triangles()) {
        Vertex& v0 = vertices[t.v0];
        Vertex& v1 = vertices[t.v1];
        Vertex& v2 = vertices[t.v2];

        // assuming flat shading only for now
        Vec3 centroid = (v0.position + v1.position + v2.position) / 3.0f;
        Vec3 normal = (v0.normal + v1.normal + v2.normal).normalize();

        Color color = Color(0x000000);
        for (Light* light : lights) {
            color += light->compute(centroid, normal, materials[v0.materialIndex]);
        }

        Vec3 p0 = (viewport * clipAndProject(v0.position, projection).toVec4()).toVec3();
        Vec3 p1 = (viewport * clipAndProject(v1.position, projection).toVec4()).toVec3();
        Vec3 p2 = (viewport * clipAndProject(v2.position, projection).toVec4()).toVec3();

        triangle(p0, p1, p2, color);
    }

    // wireframe
    // for (const auto& edge : scene.edges()) {
    //     int x0 = (int)vertices[edge.first].position.x;
    //     int y0 = (int)vertices[edge.first].position.y;
    //     int x1 = (int)vertices[edge.second].position.x;
    //     int y1 = (int)vertices[edge.second].position.y;
    //     line(x0, y0, x1, y1);
    // }
}

Vec3 Renderer::clipAndProject(const Vec3& pos, const Mat4& projection) {
    Vec3 p = (projection * pos.toVec4()).toVec3();
    clipping(p);
    return p;
}

// Writes vertex to fragment buffer
void Renderer::setFragmentColor(const Vec3 &v, Color &color) {
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

double signed_triangle_area(int x0, int y0, int x1, int y1, int x2, int y2){
    return ((y1 - y0) * (x1 + x0) + (y2 - y1) * (x2 + x1) + (y0 - y2) * (x0 + x2)) / 2;
}

void Renderer::triangle(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3, Color color){
    int xmin = std::min(std::min(p1.x, p2.x), p3.x);
    int ymin = std::min(std::min(p1.y, p2.y), p3.y);
    int xmax = std::max(std::max(p1.x, p2.x), p3.x);
    int ymax = std::max(std::max(p1.y, p2.y), p3.y);
    double total_area = signed_triangle_area(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    if (total_area < 1) return;

    // each pixel is drawn separately so we can take advantage of parallelism
    #pragma omp parallel for
    for (int x = xmin; x <= xmax; ++x){
        for (int y = ymin; y <= ymax; ++y){
            // these are the barycentric coordinates of the point (x, y)
            double alpha = signed_triangle_area(x, y, p2.x, p2.y, p3.x, p3.y) / total_area;
            double beta = signed_triangle_area(x, y, p3.x, p3.y, p1.x, p1.y) / total_area;
            double gamma = signed_triangle_area(x, y, p1.x, p1.y, p2.x, p2.y) / total_area;

            // check if the point falls outside the triangle
            if (alpha < 0 || beta < 0 || gamma < 0) continue;

            unsigned char z = alpha * p1.z + beta * p2.z + gamma * p3.z;

            // if the pixel is behind something, dont paint it
            // if (z <= zbuffer.get(x, y)[0]) continue;

            // zbuffer.set(x, y, {z});
            _fragmentBuffer[y * _window->getWidth() + x].color = color.asVec3();
        }
    }
}