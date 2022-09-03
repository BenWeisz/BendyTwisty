#pragma once

#include <glm/glm.hpp>

#include "Model.h"
#include "ShaderProgram.h"

#define MATERIAL_FLAT 0x00
#define MATERIAL_PHONG 0x01
#define MATERIAL_CUSTOM 0x02

struct Material {
    ShaderProgram& shader;
    unsigned int type;
    Material(ShaderProgram& shaderProgram, unsigned int materialType) : shader(shaderProgram), type(materialType) {}
};

struct FlatMaterial : Material {
    glm::vec3 color;
    FlatMaterial(ShaderProgram& shaderProgram, unsigned int materialType, glm::vec3 rgb) : Material(shaderProgram, materialType), color(rgb) {}
};

struct PhongMaterial : FlatMaterial {
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    PhongMaterial(ShaderProgram& shaderProgram, unsigned int materialTypem, glm::vec3 rgb, float ambient, float diffuse, float specular) : FlatMaterial(shaderProgram, materialTypem, rgb), ambientStrength(ambient), diffuseStrength(diffuse), specularStrength(specular) {}
};