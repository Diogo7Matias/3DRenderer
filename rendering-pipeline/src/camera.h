#pragma once

#include "math/vec3.h"

class Camera {
    // in World Coordinates
    Vec3 _position;
    Vec3 _direction;    // normalized
    Vec3 _up;           // normalized

public:
    Camera(Vec3 position) : _position(position) {
        _direction = Vec3(0, 0, -1);
        _up = Vec3(0, 1, 0);
    }

    Camera() {
        _position = Vec3(1, 1, 1);
        _direction = Vec3(0, 0, -1);
        _up = Vec3(0, 1, 0);
    }

    const Vec3 &position() const { return _position; }

    const Vec3 &direction() const { return _direction; }

    const Vec3 &up() const { return _up; }

    void setPosition(Vec3 position) {
        _position = position;
    }

    void lookAt(Vec3 lookAt) {
        _direction = (lookAt - _position).normalize();
    }

    void setRoll(Vec3 up) {
        // TODO
    }
};

/* .............. Orthographic Camera .............. */

class OrthographicCamera : public Camera {
    float _left, _right, _top, _bottom;
    float _near, _far;

public:
    OrthographicCamera(Vec3 position, float left, float right, float top, float bottom, float near, float far)
        : Camera(position), _left(left), _right(right), _top(top), _bottom(bottom), _near(near), _far(far) {}

    OrthographicCamera(float left, float right, float top, float bottom, float near, float far)
        : Camera(), _left(left), _right(right), _top(top), _bottom(bottom), _near(near), _far(far) {}

};

/* .............. Perspective Camera .............. */

class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(Vec3 position) : Camera(position) {}
};