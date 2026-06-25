#pragma once

#include "color.h"

class Material {
    Color _color;

    // Reflection Coefficients
    float _diffusion;
    float _specular;
    float _shininess;

public:
    Material(Color color, float diffusion) : _color(color), _diffusion(diffusion) {}
    Material(Color color, float specular, float shininess) : _color(color), _specular(specular), _shininess(shininess) {}

    virtual ~Material() = default;
};

// --------------------------------------------------------------------------

class DiffuseMaterial : public Material {

public:
    DiffuseMaterial(Color color, float diffusion) : Material(color, diffusion) {}
};