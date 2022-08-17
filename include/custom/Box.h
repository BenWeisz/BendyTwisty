#pragma once

#include "ModelFactory.h"

class Box : public Entity {
   public:
    Box() : Entity(), m_Time(0.0f) {
        m_Model = ModelFactory::ColorCube();
        m_Scale = glm::vec3(2.0f, 2.0f, 1.0f);
    }

    void Update(const float deltaTime) {
        m_Time += deltaTime;
        float radius = 2.0f;

        m_Rotation = glm::vec3(m_Time, 0.0f, 0.0f);
        m_Translation = radius * glm::vec3(cos(m_Time), sin(m_Time), 0.0f);
        // m_Translation = glm::vec3(1.0f, 0.0f, 0.0);
    }

   private:
    float m_Time;
};