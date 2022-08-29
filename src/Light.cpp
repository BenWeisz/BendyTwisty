#include "Light.h"

Light::Light(ShaderProgram* const shader, glm::vec3 color) {
    m_Model = ModelLoader::SimpleCube();
    m_Shader = shader;

    m_Transform.SetScale(0.8f, 0.8f, 0.8f);
    m_Transform.SetTranslation(0.0f, 5.0f, -5.0f);

    m_Color = color;
    m_AmbientStrength = 0.1f;
    m_SpecularStrength = 0.5f;

    m_Time = 0.0f;
}

void Light::Draw(const float deltaTime) const {
    assert(m_Model != nullptr);
    m_Shader->SetUniform3fv("u_FlatColor", &m_Color[0]);

    m_Model->Bind();
    m_Model->Draw();
    m_Model->Unbind();
}

void Light::Update(const float deltaTime) {
    // m_Time += deltaTime;
    m_Transform.SetTranslation(5.0f * cos(m_Time), 1.0f, 5.0f * sin(m_Time));
}

ShaderProgram* Light::GetShader() const {
    return m_Shader;
}

Transform Light::GetTransform() const {
    return m_Transform;
}

glm::vec3 Light::GetColor() const {
    return m_Color;
}

float Light::GetAmbientStrength() const {
    return m_AmbientStrength;
}

float Light::GetSpecularStrength() const {
    return m_SpecularStrength;
}

void Light::SetColor(const float* rgb) {
    m_Color = glm::make_vec3(rgb);
}