#pragma once

#include <glm/ext.hpp>

#include "Light.h"
#include "ModelLoader.h"
#include "Transform.h"
#include "Model.h"
#include "ShaderProgram.h"

class PointLight : public Light {
   public:
    PointLight(ShaderProgram* const shader, glm::vec3 color);
    void Draw(const float deltaTime) const;
    void Update(const float deltaTime);
    Transform GetTransform() const;

   private:
    Model* m_Model;
    Transform m_Transform;
    float m_Time;
};