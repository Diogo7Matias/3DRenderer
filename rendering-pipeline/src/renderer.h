#pragma once

#include "scene.h"
#include "camera.h"
#include "window.h"
#include "color.h"

struct Fragment {
    Vec3 position;
    Vec3 color;
};

class Renderer {
    Window* _window;
    Fragment *_fragmentBuffer;

public:
    Renderer(Window* window) : _window(window) {
        _fragmentBuffer = new Fragment[_window->getWidth() * _window->getHeight()];
        for (size_t i = 0; i < _window->getWidth() * _window->getHeight(); ++i) {
            _fragmentBuffer[i].color = Color(0x000000).asVec3(); // black pixel
        }
    }

public:
    void render(const Scene &scene, const Camera &camera);

    Fragment* fragmentBuffer() const { return _fragmentBuffer; }

private:
    void setFragmentColor(const Vec3 &v, Color &color);
    Vec3 clipAndProject(const Vec3& pos, const Mat4& projection);
    void clipping(Vec3 &v);
    void line(int x0, int y0, int x1, int y1);
    void triangle(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3, Color color);
};