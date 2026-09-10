# 3D Renderer

A 3D renderer written in C++, built without graphics APIs, for learning purposes. Therefore, the entire rendering pipeline runs on the CPU.

This project implements a classical geometry pipeline with scene and camera management, view and projection matrices (both perspective and orthographic), Blinn-Phong illumination with ambient and point lights, flat and Gouraud shading, etc...    
Geometry can be defined programmatically or loaded from OBJ files.    

SDL3 is used for opening a window and displaying the framebuffer. 

### The renderer can be split into three main layers:

- **Application** - Contains application logic. Allows the user to create a 3D scene with objects, lights and cameras.

- **Geometry Processing** - Handles mathematical operations such as coordinate space transformations, camera/view transformations, clipping, and lighting calculations.

- **Rasterization** - Converts processed data into pixels and draws the final scene onto the screen. Makes use of SDL3 to assist in the final steps.

## Build and run

Run the following after cloning this repo
```s
git submodule update --init
```

Navigate to `/rendering-pipeline`
```s
cmake -S . -B build
cmake --build build
```

Then, inside `/build`
```s
make
./rendering-pipeline
```
