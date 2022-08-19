#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(GLFWwindow* window, const size_t width, const size_t height) : m_Window(window) {
    glm::vec3 initialEye = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 initialCenter = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 initialUp = glm::vec3(0.0f, 1.0f, 0.0f);

    m_Camera = new Camera(width, height, initialEye, initialCenter, initialUp);
}

ModelRenderer::~ModelRenderer() {
    delete m_Camera;
}

void ModelRenderer::Draw(const float deltaTime) {
    for (auto& e : m_EntityShaderPairs) {
        e.shader->Bind();
        glm::mat4 mvp = m_Camera->GetCameraMatrix() * e.entity->GetTransform().GetMatrix();
        e.shader->SetUniformMat4fv("u_MVP", mvp);

        if (e.entity->IsLightingEnabled()) {
            e.entity->Draw(deltaTime, m_Light);
        } else {
            e.entity->Draw(deltaTime);
        }
        e.shader->Unbind();
    }
}

void ModelRenderer::Update(const float deltaTime) {
    m_Camera->ProcessInput(m_Window, deltaTime);

    for (auto& e : m_EntityShaderPairs) {
        e.entity->Update(deltaTime);
    }
}

void ModelRenderer::AddEntityShaderPair(Entity* entity, ShaderProgram* shader) {
    m_EntityShaderPairs.emplace_back(EntityShader{entity, shader});
}

void ModelRenderer::SetLight(Light* light) {
    m_Light = light;
}