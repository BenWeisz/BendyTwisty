#include "Entity.h"

unsigned int Entity::m_EntityNum = 0;

Entity::Entity(const std::string& name) {
    m_Name = "E" + std::to_string(m_EntityNum++) + ": " + name;
    m_Model = nullptr;
    m_Transform.SetTranslation(0.0f, 0.0f, 0.0f);
    m_Transform.SetRotation(0.0f, 0.0f, 0.0f);
    m_Transform.SetScale(1.0f, 1.0f, 1.0f);

    m_IsLightingEnabled = false;

    m_Metadata[ENTITY_SHOW_IN_GUI] = ENTITY_STATE_ENABLED;
    m_Metadata[ENTITY_SHOW_LIGHTING_IN_GUI] = ENTITY_STATE_ENABLED;
    m_Metadata[ENTITY_SHOW_COLOR_IN_GUI] = ENTITY_STATE_ENABLED;
    m_Metadata[ENTITY_SHOW_TRANSLATION_IN_GUI] = ENTITY_STATE_DISABLED;
    m_Metadata[ENTITY_SHOW_ROTATION_IN_GUI] = ENTITY_STATE_DISABLED;
    m_Metadata[ENTITY_SHOW_SCALE_IN_GUI] = ENTITY_STATE_DISABLED;

    m_ActiveMaterialIndex = -1;
}

Entity::~Entity() {
    if (m_Model != nullptr)
        delete m_Model;
}

void Entity::Draw(const float deltaTime) const {
    // ! DO NOT CHANGE MATERIALS IN DRAW CALLS
    assert(m_Model != nullptr);
    assert(m_ActiveMaterialIndex != -1);
    m_Model->Bind();

    Material* material = GetActiveMaterial();
    if (material->type == MATERIAL_FLAT) {
        material->shader.SetUniform3fv("u_FlatColor", &(((FlatMaterial*)material)->color)[0]);
    }

    m_Model->Draw();
    m_Model->Unbind();
}

void Entity::Draw(const float deltaTime, Light* const light, Camera* const camera) const {
    // ! DO NOT CHANGE MATERIALS IN DRAW CALLS
    assert(m_Model != nullptr);
    assert(m_ActiveMaterialIndex != -1);
    m_Model->Bind();
    m_Model->Draw();
    m_Model->Unbind();
}

void Entity::Update(const float deltaTime) {
}

Transform& Entity::GetTransform() {
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

void Entity::SetIsLightingEnabled(const bool isLightingEnabled) {
    unsigned int type = isLightingEnabled ? MATERIAL_PHONG : MATERIAL_FLAT;

    int index = GetMaterialIndex(type);
    assert(index != -1);

    SetActiveMaterial(index);

    m_IsLightingEnabled = isLightingEnabled;
}

void Entity::AddMaterial(Material* material) {
    m_ActiveMaterialIndex = m_Materials.size();
    m_Materials.push_back(material);
}

void Entity::SetActiveMaterial(int activeIndex) {
    if (activeIndex >= m_Materials.size()) {
        std::cerr << "Cannot set entity material index to: " << activeIndex << std::endl;
        return;
    }

    m_ActiveMaterialIndex = activeIndex;
}

Material* Entity::GetActiveMaterial() const {
    return m_Materials.at(m_ActiveMaterialIndex);
}

int Entity::GetMaterialIndex(unsigned int type) const {
    return GetMaterialIndex(type, 1);
}

int Entity::GetMaterialIndex(unsigned int type, int order) const {
    assert(order > 0);
    int occurance = 1;
    int index = 0;
    for (auto& material : m_Materials) {
        if (material->type == type) {
            if (occurance == order)
                break;
            occurance++;
        }
        index++;
    }

    if (index == m_Materials.size())
        return -1;

    return index;
}