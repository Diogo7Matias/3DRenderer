# 3DRenderer

A C++ project serving as an introduction to graphics programming. <br>
It covers fundamental concepts for generating 3D images.

<i> I intend to expand this project over time. </i>

## This Project Contains:
- 3D Rendering Pipeline
- RayTracing Algorithm

## Rendering Pipeline
It's divided into three main layers:

- Application - Contains application logic. Allows the user to create a 3D scene with objects, lights and cameras.

- Geometry Processing - Handles mathematical operations such as coordinate space transformations, camera/view transformations, clipping, and lighting calculations.

- Rasterization - Converts processed data into pixels and draws the final scene onto the screen. Makes use of SDL3 to assist in the final steps.

### To run

Inside `/build`
```s
make
./rendering-pipeline
```

## RayTracer

A simple recursive ray tracer that renders a scene of spheres with basic lighting, shadows, reflection, and refraction.

- Sphere-based ray–object intersection
- Diffuse shading with point light sources
- Hard shadows via shadow-ray checks
- Recursive reflections and refractions (limited depth)
- Fresnel-inspired blending between reflection/refraction
- PPM image output via simple perspective camera