#pragma once

#include "math/vec3.h"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    int materialIndex = -1; // -1 means unassigned

    Vertex(Vec3 position, Vec3 normal, int materialIndex) : position(position), normal(normal), materialIndex(materialIndex) {}
    Vertex(Vec3 position, Vec3 normal) : position(position), normal(normal) {}
};