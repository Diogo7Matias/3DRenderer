#pragma once

#include "scene.h"
#include "camera.h"

class Renderer {

public:
    Renderer() {}

public:
    void render(const Scene &scene, const Camera &camera);

private:
    Vec4 toCameraCoordinates(Vec4 vertex, const Camera &camera);
};