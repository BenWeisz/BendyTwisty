#pragma once

#include <glm/ext.hpp>

#include "Light.h"
#include "ModelLoader.h"
#include "Transform.h"
#include "Model.h"
#include "ShaderProgram.h"

class PointLight : public Light {
   public:
    PointLight(ShaderProgram* const shader, glm::vec3 color, const std::string& name);
    void Draw(const float deltaTime) const;
    void Update(const float deltaTime);
    ShaderProgram* GetShader() const;
    Transform GetTransform() const;

   private:
    ShaderProgram* m_Shader;
    Model* m_Model;
    Transform m_Transform;
    float m_Time;
};