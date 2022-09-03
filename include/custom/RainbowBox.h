#pragma once

#include "ModelLoader.h"

#include <iostream>

class RainbowBox : public Entity {
   public:
    RainbowBox() : Entity("RainbowBox"), m_Time(0.0f) {
        m_Model = ModelLoader::RainbowCube();
        m_Metadata[ENTITY_SHOW_LIGHTING_IN_GUI] = ENTITY_STATE_DISABLED;
        m_Metadata[ENTITY_SHOW_COLOR_IN_GUI] = ENTITY_STATE_DISABLED;
        m_Metadata[ENTITY_SHOW_TRANSLATION_IN_GUI] = ENTITY_STATE_ENABLED;
        m_Metadata[ENTITY_SHOW_ROTATION_IN_GUI] = ENTITY_STATE_ENABLED;
        m_Metadata[ENTITY_SHOW_SCALE_IN_GUI] = ENTITY_STATE_ENABLED;
        m_IsLightingEnabled = false;
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