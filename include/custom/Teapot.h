#pragma once

#include <glm/ext.hpp>

#include "ModelLoader.h"
#include "Entity.h"

#include "PointLight.h"
#include "DirectionalLight.h"

class Teapot : public Entity {
   public:
    Teapot() : Entity("Teapot") {
        m_Model = ModelLoader::LoadModel("../res/obj/teapot_area.obj");
        m_Transform.SetTranslation(-5.0f, 0.0f, 0.0f);
        m_Transform.SetScale(0.5, 0.5f, 0.5f);
    }

    void Draw(const float deltaTime, Light* const light, Camera* const camera) const override {
        assert(m_Model != nullptr);
        assert(m_ActiveMaterialIndex != -1);
        m_Model->Bind();

        glm::mat4 model = m_Transform.GetMatrix();
        glm::mat4 normalModel = glm::transpose(glm::inverse(model));

        PhongMaterial* phongMaterial = (PhongMaterial*)GetActiveMaterial();
        ShaderProgram& shader = phongMaterial->shader;

        shader.SetUniformMat4fv("u_Model", model);
        shader.SetUniformMat4fv("u_NormalModel", normalModel);

        if (light->GetType() == LIGHT_POINT) {
            PointLight* pointLight = (PointLight*)light;
            shader.SetUniform3fv("u_LightPos", &pointLight->GetTransform().GetTranslation()[0]);
        } else if (light->GetType() == LIGHT_DIRECTIONAL) {
            DirectionalLight* directionalLight = (DirectionalLight*)light;
            shader.SetUniform3fv("u_LightDir", &directionalLight->GetDirection()[0]);
        }

        shader.SetUniform3fv("u_LightColor", &light->GetColor()[0]);
        shader.SetUniform3fv("u_ViewPos", &camera->GetEye()[0]);

        shader.SetUniform3fv("u_FlatColor", &(phongMaterial->color)[0]);
        shader.SetUniform1f("u_AmbientStrength", phongMaterial->ambientStrength);
        shader.SetUniform1f("u_DiffuseStrength", phongMaterial->diffuseStrength);
        shader.SetUniform1f("u_SpecularStrength", phongMaterial->specularStrength);

        m_Model->Draw();
        m_Model->Unbind();
    }
};