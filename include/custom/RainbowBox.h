#pragma once

#include "ModelFactory.h"

class RainbowBox : public Entity {
   public:
    RainbowBox() : Entity(), m_Time(0.0f) {
        m_Model = ModelFactory::RainbowCube();
        m_Transform.SetScale(1.0f, 1.0f, 1.0f);
        m_LightingEnabled = false;
    }

    void Update(const float deltaTime) {
        m_Time += deltaTime;
        float radius = 2.0f;

        m_Transform.SetRotation(m_Time, 0.0f, 0.0f);
        m_Transform.SetTranslation(radius * cos(m_Time), radius * sin(m_Time), 0.0f);
    }

   private:
    float m_Time;
};