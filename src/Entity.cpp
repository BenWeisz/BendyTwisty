#include "Entity.h"

#include <iostream>

Entity::Entity() {
    m_Model = nullptr;
    m_Translation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Entity::~Entity() {
    if (m_Model != nullptr)
        delete m_Model;
}

void Entity::Draw(const float deltaTime) const {
    assert(m_Model != nullptr);
    m_Model->Bind();
    m_Model->Draw();
    m_Model->Unbind();
}

void Entity::Update(const float deltaTime) {
}

glm::mat4 Entity::GetTransform() const {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, m_Translation);

    transform = glm::rotate(transform, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, m_Rotation.y, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::rotate(transform, m_Rotation.z, glm::vec3(1.0f, 0.0f, 0.0f));

    transform = glm::scale(transform, m_Scale);

    return transform;
}