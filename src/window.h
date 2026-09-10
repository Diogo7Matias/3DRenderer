#pragma once

#include <SDL3/SDL.h>
#include "math/mat4.h"

class Window {
private:
    SDL_Window* sdl_window;
    int width;
    int height;

public:
    Window(const char* title, int w, int h) : width(w), height(h) {
        sdl_window = SDL_CreateWindow(title, w, h, 0);
    }

    ~Window() {
        SDL_DestroyWindow(sdl_window);
    }

    SDL_Window* getSDLWindow() const { return sdl_window; }

    int getWidth() const { return width; }

    int getHeight() const { return height; }

    Mat4 viewportMatrix() const;
};
