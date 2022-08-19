#include "Entity.h"

#include <iostream>

Entity::Entity() {
    m_Model = nullptr;
    m_Transform.SetTranslation(0.0f, 0.0f, 0.0f);
    m_Transform.SetRotation(0.0f, 0.0f, 0.0f);
    m_Transform.SetScale(1.0f, 1.0f, 1.0f);
    m_LightingEnabled = true;
}

Entity::~Entity() {
    if (m_Model != nullptr)
        delete m_Model;
}

void Entity::Draw(const float deltaTime, ShaderProgram* const shader) const {
    assert(m_Model != nullptr);
    m_Model->Bind();
    m_Model->Draw();
    m_Model->Unbind();
}

void Entity::Draw(const float deltaTime, ShaderProgram* const shader, Light* const light) const {
    assert(m_Model != nullptr);
    m_Model->Bind();
    m_Model->Draw();
    m_Model->Unbind();
}

void Entity::Update(const float deltaTime) {
}

Transform Entity::GetTransform() const {
    return m_Transform;
}

bool Entity::IsLightingEnabled() const {
    return m_LightingEnabled;
}