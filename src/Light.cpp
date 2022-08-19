#include "Light.h"

Light::Light(ShaderProgram* const shader, glm::vec3 color) {
    m_Model = ModelFactory::SimpleCube();
    m_Shader = shader;

    m_Transform.SetScale(0.8f, 0.8f, 0.8f);
    m_Transform.SetTranslation(2.0f, 0.0f, 0.0f);
    m_Color = color;
}

void Light::Draw(const float deltaTime) const {
    assert(m_Model != nullptr);
    m_Shader->SetUniform3fv("u_FlatColor", &m_Color[0]);

    m_Model->Bind();
    m_Model->Draw();
    m_Model->Unbind();
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