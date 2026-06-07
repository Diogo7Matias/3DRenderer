#include <SDL3/SDL.h>

#include "math/vec3.h"
#include "renderer.h"
#include "scene.h"
#include "geometry.h"
#include "camera.h"

/////////////////////////////////////////////////////////////////////////
//                           Aplication Logic                          //
/////////////////////////////////////////////////////////////////////////

const int WIDTH  = 800;
const int HEIGHT = 600;

Renderer renderer = Renderer();
Scene scene = Scene();
Camera currentCamera;

void createGeometry() {
    Vec3 pos = Vec3(1, 1, 1);
    Geometry::Cube cube = Geometry::Cube(pos, 2);
    
    scene.add(cube);
}

void createCameras() {
    Vec3 pos = Vec3(1, 0, 0);

    Camera camera1 = OrthographicCamera(pos, -10, 10, 10, -10, 0.1, 100);
    scene.add(camera1);

    Camera camera2 = OrthographicCamera(-10, 10, 10, -10, 0.1, 100);
    scene.add(camera2);

    currentCamera = camera1;
}

void createLights() {
    // TODO
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Renderer", WIDTH, HEIGHT, 0);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, NULL);
    SDL_Texture* texture = SDL_CreateTexture(
        sdl_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    uint32_t framebuffer[WIDTH * HEIGHT];

    bool running = true;
    SDL_Event event;

    createGeometry();
    createCameras();
    createLights();

    renderer.render(scene, currentCamera);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // clear to black
        memset(framebuffer, 0, sizeof(framebuffer));

        SDL_UpdateTexture(texture, NULL, framebuffer, WIDTH * sizeof(uint32_t));
        SDL_RenderClear(sdl_renderer);
        SDL_RenderTexture(sdl_renderer, texture, NULL, NULL);
        SDL_RenderPresent(sdl_renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}