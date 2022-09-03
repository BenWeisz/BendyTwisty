#pragma once

#include "Light.h"

class DirectionalLight : public Light {
   public:
    DirectionalLight(glm::vec3 color, glm::vec3 direction, const std::string& name);
    glm::vec3 GetDirection() const;

   private:
    glm::vec3 m_Direction;
};