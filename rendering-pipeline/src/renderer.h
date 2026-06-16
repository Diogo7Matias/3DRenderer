#pragma once

#include "scene.h"
#include "camera.h"
#include "window.h"

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
            _fragmentBuffer[i].color = Vec3(0, 0, 0); // black pixel
        }
    }

public:
    void render(const Scene &scene, const Camera &camera);

    Fragment* fragmentBuffer() const { return _fragmentBuffer; }

private:
    void clipping(Vec3 &v);
    void line(int x0, int y0, int x1, int y1);
};