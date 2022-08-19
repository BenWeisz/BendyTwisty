#include "Light.h"

Light::Light() {
    m_Model = ModelFactory::SimpleCube();
    m_Transform.SetTranslation(2.0f, 0.0f, 0.0f);
    m_Color = glm::vec3(1.0f, 1.0f, 1.0f);
}