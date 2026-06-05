#pragma once

#include "math/vec3.h"

class Camera {
    Vec3 _position;

public:
    Camera(Vec3 position) : _position(position) {}
};

/* .............. Orthographic Camera .............. */

class OrthographicCamera : public Camera {

public:
    OrthographicCamera(Vec3 position) : Camera(position) {}
};

/* .............. Perspective Camera .............. */

class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(Vec3 position) : Camera(position) {}
};