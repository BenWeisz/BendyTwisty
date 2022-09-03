#include "Light.h"

unsigned int Light::m_LightNum = 0;

Light::Light(glm::vec3 color, unsigned int type, const std::string& name) {
    m_Color = color;
    m_Type = type;
    m_Name = "L" + std::to_string(m_LightNum++) + ": " + name;
}

glm::vec3 Light::GetColor() const {
    return m_Color;
}

void Light::SetColor(const float* rgb) {
    m_Color = glm::make_vec3(rgb);
}

const unsigned int Light::GetType() const {
    return m_Type;
}

std::string& Light::GetName() {
    return m_Name;
}