#pragma once

#include <extern/glm/glm.hpp>

#include "ModelFactory.h"

class Plane : public Entity {
   public:
    Plane() : Entity() {
        m_Model = ModelFactory::SimplePlane();
        m_Transform.SetScale(10.0f, 10.0f, 1.0f);
        m_Transform.SetRotation(90.0f, 0.0f, 0.0f);
        m_Transform.SetTranslation(0.0f, -1.0f, 0.0f);
        m_Color = glm::vec3(0.498f, 0.588f, 0.6f);
    }

    void Draw(const float deltaTime, ShaderProgram* const shader, Light* const light) const override {
        assert(m_Model != nullptr);
        m_Model->Bind();

        shader->SetUniform3fv("u_FlatColor", &m_Color[0]);
        m_Model->Draw();

        m_Model->Unbind();
    }

   private:
    glm::vec3 m_Color;
};