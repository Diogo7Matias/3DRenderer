#include <SDL3/SDL.h>
#include <memory>
#include <iostream>

#include "math/vec3.h"
#include "window.h"
#include "renderer.h"
#include "scene.h"
#include "geometry.h"
#include "material.h"
#include "mesh.h"
#include "color.h"
#include "camera.h"

/////////////////////////////////////////////////////////////////////////
//                           Aplication Logic                          //
/////////////////////////////////////////////////////////////////////////

const int WIDTH  = 800;
const int HEIGHT = 600;

Scene scene = Scene();

void createObjects() {
    Vec3 pos = Vec3(1, 0, -2);

    Geometry::Cube cube = Geometry::Cube(pos, 3);
    Geometry::Sphere sphere = Geometry::Sphere(pos, 1);
    Material mat = DiffuseMaterial(0xFFFFFFFF, 1);

    Mesh mesh = Mesh(sphere, mat);
    scene.add(mesh);
}

void createCameras() {
    Vec3 pos = Vec3(1, 0, 0);

    std::unique_ptr<Camera> camera1 = std::make_unique<PerspectiveCamera>(pos, 90, (float)WIDTH / (float)HEIGHT, 0.1, 100);
    scene.add(std::move(camera1));

    std::unique_ptr<Camera> camera2 = std::make_unique<OrthographicCamera>(pos, -10, 10, 10, -10, 0.1, 100);
    scene.add(std::move(camera2));
}

void createLights() {
    Vec3 pos = Vec3(2, 2, 2);

    std::unique_ptr<Light> light1 = std::make_unique<AmbientLight>(Color(0xf300ff));
    scene.add(std::move(light1));
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    Window window("Renderer", WIDTH, HEIGHT);
    uint32_t framebuffer[WIDTH * HEIGHT];
    Renderer renderer(&window);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window.getSDLWindow(), NULL);
    SDL_Texture* texture = SDL_CreateTexture(
        sdl_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    bool running = true;
    SDL_Event event;
    int cameraIndex = 0;

    createObjects();
    createCameras();
    createLights();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
                if (event.key.key == SDLK_SPACE) {
                    cameraIndex = (cameraIndex + 1) % scene.cameraCount();
                }
            }
            if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                float factor = (event.wheel.y < 0) ? 0.9 : 1.1;
                scene.getCamera(cameraIndex).zoom(factor);
            }
        }

        // clear to black
        memset(framebuffer, 0, sizeof(framebuffer));

        renderer.render(scene, scene.getCamera(cameraIndex));
        Fragment* fragments = renderer.fragmentBuffer();

        // only write fragments that have been set (non-black color)
        for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
            if (fragments[i].color.x > 0 || fragments[i].color.y > 0 || fragments[i].color.z > 0) {
                framebuffer[i] = Color::fromVec3(fragments[i].color).value();
            }
        }

        SDL_UpdateTexture(texture, NULL, framebuffer, WIDTH * sizeof(uint32_t));
        SDL_RenderClear(sdl_renderer);
        SDL_RenderTexture(sdl_renderer, texture, NULL, NULL);
        SDL_RenderPresent(sdl_renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_Quit();
    return 0;
}