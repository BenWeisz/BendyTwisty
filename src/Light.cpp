#include "Light.h"

Light::Light(ShaderProgram* const shader) {
    m_Model = ModelFactory::SimpleCube();
    m_Shader = shader;

    m_Transform.SetTranslation(2.0f, 0.0f, 0.0f);
    m_Color = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Light::Draw(const float deltaTime) const {
    assert(m_Model != nullptr);
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