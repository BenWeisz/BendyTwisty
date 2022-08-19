#pragma once

#include "ModelFactory.h"

class Light : public Entity {
   public:
    Light() : Entity() {
        m_Model = ModelFactory::SimpleCube();
        m_Translation = glm::vec3(2.0f, 0.0f, 0.0f);
    }
};