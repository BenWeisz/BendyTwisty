#pragma once

#include "ModelLoader.h"

#include <iostream>

class RainbowBox : public Entity {
   public:
    RainbowBox() : Entity("RainbowBox"), m_Time(0.0f) {
        m_Model = ModelLoader::RainbowCube();
        m_Transform.SetScale(1.0f, 1.0f, 1.0f);
        m_IsLightingEnabled = false;
        m_Metadata[ENTITY_SHOW_TRANSFORM_IN_GUI] = ENTITY_STATE_ENABLED;
        m_Metadata[ENTITY_SHOW_ROTATION_IN_GUI] = ENTITY_STATE_ENABLED;
    }

    void Update(const float deltaTime) override {
        m_Time += deltaTime;
        // float radius = 2.0f;

        // m_Transform.SetRotation(m_Time, 0.0f, 0.0f);
        // m_Transform.SetTranslation(radius * cos(m_Time), radius * sin(m_Time), 0.0f);
    }

   private:
    float m_Time;
};