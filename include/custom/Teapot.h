#pragma once

#include <glm/ext.hpp>

#include "ModelLoader.h"
#include "Entity.h"

class Teapot : public Entity {
   public:
    Teapot() : Entity() {
        m_Model = ModelLoader::LoadModel("../res/obj/string_area.obj");
        // m_Transform.SetTranslation(-5.0f, 0.0f, 0.0f);
        m_Transform.SetScale(0.5, 0.5f, 0.5f);

        m_Color = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    void Draw(const float deltaTime, ShaderProgram* const shader, PointLight* const pointLight, Camera* const camera) const override {
        assert(m_Model != nullptr);
        m_Model->Bind();

        glm::mat4 model = m_Transform.GetMatrix();
        glm::mat4 normalModel = glm::transpose(glm::inverse(model));

        shader->SetUniformMat4fv("u_Model", model);
        shader->SetUniformMat4fv("u_NormalModel", normalModel);

        shader->SetUniform3fv("u_LightPos", &pointLight->GetTransform().GetTranslation()[0]);
        shader->SetUniform3fv("u_LightColor", &pointLight->GetColor()[0]);
        shader->SetUniform3fv("u_ViewPos", &camera->GetEye()[0]);

        shader->SetUniform3fv("u_FlatColor", &m_Color[0]);
        shader->SetUniform1f("u_AmbientStrength", pointLight->GetAmbientStrength());
        shader->SetUniform1f("u_SpecularStrength", pointLight->GetSpecularStrength());

        m_Model->Draw();
        m_Model->Unbind();
    }

    void SetColor(const float* color) {
        m_Color = glm::make_vec3(color);
    }

   private:
    glm::vec3 m_Color;
};