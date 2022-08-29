#pragma once

#include <glm/ext.hpp>

#include "ModelLoader.h"
#include "Transform.h"
#include "Model.h"
#include "ShaderProgram.h"

#define LIGHT_POINT 0
#define LIGHT_DIRECTIONAL 1

class Light {
   public:
    Light(glm::vec3 color, unsigned int type);
    glm::vec3 GetColor() const;
    float GetAmbientStrength() const;
    float GetSpecularStrength() const;
    void SetColor(const float* rgb);
    const unsigned int GetType() const;

   protected:
    glm::vec3 m_Color;
    float m_AmbientStrength;
    float m_SpecularStrength;
    unsigned int m_Type;
};