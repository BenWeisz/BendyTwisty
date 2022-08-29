#pragma once

#include <glm/ext.hpp>

#include "ModelLoader.h"
#include "Transform.h"
#include "Model.h"
#include "ShaderProgram.h"

class Light {
   public:
    Light(ShaderProgram* const shader, glm::vec3 color);
    ShaderProgram* GetShader() const;
    glm::vec3 GetColor() const;
    float GetAmbientStrength() const;
    float GetSpecularStrength() const;
    void SetColor(const float* rgb);

   protected:
    ShaderProgram* m_Shader;
    glm::vec3 m_Color;
    float m_AmbientStrength;
    float m_SpecularStrength;
};