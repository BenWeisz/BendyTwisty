/*
#include "Entity.h"

Entity::Entity(const Model* model) : m_Model(model) {
    m_Translation = glm::mat4(0.0f);
    m_Translation[3][3] = 1.0f;

    m_Rotation = glm::mat4(1.0f);

    m_Scale = glm::mat4(1.0f);
}

Entity::~Entity() {
    if (m_Model != nullptr)
        delete m_Model;
}

void Entity::Draw() const {
    m_Model->Bind();
    m_Model->Draw();
    m_Model->Unbind();
}

glm::mat4 Entity::GetTransform() const {
    return m_Translation * m_Rotation * m_Scale;
}*/