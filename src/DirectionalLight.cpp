#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction) : Light(color, LIGHT_DIRECTIONAL) {
    m_Direction = direction;
}

glm::vec3 DirectionalLight::GetDirection() const {
    return m_Direction;
}