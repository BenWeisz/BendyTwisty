#include "Entity.h"

int Entity::m_EntityNum = 0;

Entity::Entity(const std::string& name) {
    m_Name = "E" + std::to_string(m_EntityNum++) + ": " + name;
    m_Model = nullptr;
    m_Transform.SetTranslation(0.0f, 0.0f, 0.0f);
    m_Transform.SetRotation(0.0f, 0.0f, 0.0f);
    m_Transform.SetScale(1.0f, 1.0f, 1.0f);

    m_IsLightingEnabled = true;

    m_Metadata[ENTITY_SHOW_IN_GUI] = ENTITY_STATE_ENABLED;
    m_Metadata[ENTITY_SHOW_LIGHTING_IN_GUI] = ENTITY_STATE_ENABLED;
    m_Metadata[ENTITY_SHOW_COLOR_IN_GUI] = ENTITY_STATE_ENABLED;
    m_Metadata[ENTITY_SHOW_TRANSFORM_IN_GUI] = ENTITY_STATE_DISABLED;
    m_Metadata[ENTITY_SHOW_ROTATION_IN_GUI] = ENTITY_STATE_DISABLED;
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

void Entity::Draw(const float deltaTime, ShaderProgram* const shader, Light* const light, Camera* const camera) const {
    assert(m_Model != nullptr);
    m_Model->Bind();
    m_Model->Draw();
    m_Model->Unbind();
}

void Entity::Update(const float deltaTime) {
}

const Transform& Entity::GetTransform() const {
    return m_Transform;
}

char Entity::GetMetadata(const char key) const {
    return m_Metadata.at(key);
}

const std::string& Entity::GetName() const {
    return m_Name;
}

bool Entity::GetIsLightingEnabled() const {
    return m_IsLightingEnabled;
}