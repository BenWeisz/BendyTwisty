#pragma once

#include <glm/ext.hpp>

#include "ModelImporter.h"
#include "Entity.h"

class Teapot : public Entity {
   public:
    Teapot() : Entity() {
        m_Model = ModelImporter::LoadModel("../res/obj/teapot.obj");
        m_Transform.SetTranslation(-5.0f, 0.0f, 0.0f);

        m_Color = glm::vec3(1.0f, 0.0f, 0.0f);
        m_LightingEnabled = false;
    }

    void Draw(const float deltaTime, ShaderProgram* const shader) const override {
        assert(m_Model != nullptr);
        m_Model->Bind();

        shader->SetUniform3fv("u_FlatColor", &m_Color[0]);

        m_Model->Draw();
        m_Model->Unbind();
    }

    void SetColor(const float* color) {
        m_Color = glm::make_vec3(color);
    }

   private:
    glm::vec3 m_Color;
};