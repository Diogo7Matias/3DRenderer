#pragma once

#include "math/vec3.h"
#include "math/mat4.h"

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

    virtual ~Camera() = default;

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

    Mat4 viewMatrix() const {
        Vec3 v = up();
        Vec3 n = direction();
        Vec3 u = n.cross(v).normalize();

        Mat4 translation = Mat4::translation(-position());
        Mat4 rotation = Mat4::fromBasis(u, v, -n);
        Mat4 scale = Mat4::scale(Vec3(1, 1, -1));
        return scale * rotation * translation;
    }

    virtual Mat4 projectionMatrix() const = 0;
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

    float near() const { return _near; }

    float far() const { return _far; }

    float left() const { return _left; }

    float right() const { return _right; }

    float top() const { return _top; }
    
    float bottom() const { return _bottom; }

    Mat4 projectionMatrix() const override {
        Mat4 translation = Mat4::translation(Vec3(0, 0, -near()));
        Mat4 scale = Mat4::scale(Vec3(1 / left(), 1 / top(), 1 / (far() - near())));
        return scale * translation;
    }

};

/* .............. Perspective Camera .............. */

class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(Vec3 position) : Camera(position) {}

    Mat4 projectionMatrix() const override {
        // TODO
        return Mat4::identity();
    }
};