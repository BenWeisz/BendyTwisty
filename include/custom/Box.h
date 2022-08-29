#pragma once

#include <glm/ext.hpp>

#include "ModelLoader.h"
#include "Entity.h"

class Box : public Entity {
   public:
    Box() : Entity() {
        m_Model = ModelLoader::NormalsCube();
        m_Transform.SetTranslation(5.0f, 0.0f, 0.0f);

        m_Color = glm::vec3(0.498f, 0.588f, 0.6f);
    }

    void Draw(const float deltaTime, ShaderProgram* const shader, Light* const light, Camera* const camera) const override {
        assert(m_Model != nullptr);
        m_Model->Bind();

        glm::mat4 model = m_Transform.GetMatrix();
        glm::mat4 normalModel = glm::transpose(glm::inverse(model));

        shader->SetUniformMat4fv("u_Model", model);
        shader->SetUniformMat4fv("u_NormalModel", normalModel);

        shader->SetUniform3fv("u_LightPos", &light->GetTransform().GetTranslation()[0]);
        shader->SetUniform3fv("u_LightColor", &light->GetColor()[0]);
        shader->SetUniform3fv("u_ViewPos", &camera->GetEye()[0]);

        shader->SetUniform3fv("u_FlatColor", &m_Color[0]);
        shader->SetUniform1f("u_AmbientStrength", light->GetAmbientStrength());
        shader->SetUniform1f("u_SpecularStrength", light->GetSpecularStrength());

        m_Model->Draw();
        m_Model->Unbind();
    }

    void SetColor(const float* color) {
        m_Color = glm::make_vec3(color);
    }

   private:
    glm::vec3 m_Color;
};