#pragma once

#include <extern/glm/glm.hpp>

#include "ModelFactory.h"
#include "Transform.h"
#include "Model.h"
#include "ShaderProgram.h"

class Light {
   public:
    Light(ShaderProgram* const shader, glm::vec3 color);
    void Draw(const float deltaTime) const;
    void Update(const float deltaTime);
    ShaderProgram* GetShader() const;
    Transform GetTransform() const;
    glm::vec3 GetColor() const;
    float GetAmbientStrength() const;
    float GetSpecularStrength() const;

   private:
    Model* m_Model;
    ShaderProgram* m_Shader;
    Transform m_Transform;
    glm::vec3 m_Color;
    float m_AmbientStrength;
    float m_SpecularStrength;
    float m_Time;
};