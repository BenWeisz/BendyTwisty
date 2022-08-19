#pragma once

#include <extern/glm/glm.hpp>

#include "ModelFactory.h"
#include "Transform.h"
#include "Model.h"

class Light {
   public:
    Light();

   private:
    Model* m_Model;
    Transform m_Transform;
    glm::vec3 m_Color;
};