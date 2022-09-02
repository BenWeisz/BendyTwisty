#pragma once

#include <glm/glm.hpp>

#include "Model.h"
#include "ShaderProgram.h"

struct Material {
    ShaderProgram& shader;
    Material(ShaderProgram& shaderProgram) : shader(shaderProgram) {}
};

struct FlatMaterial : Material {
    glm::vec3 color;
    FlatMaterial(ShaderProgram& shaderProgram, glm::vec3 rgb) : Material(shaderProgram), color(rgb) {}
};

struct PhongMaterial : FlatMaterial {
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    PhongMaterial(ShaderProgram& shaderProgram, glm::vec3 rgb, float ambient, float diffuse, float specular) : FlatMaterial(shaderProgram, rgb), ambientStrength(ambient), diffuseStrength(diffuse), specularStrength(specular) {}
};