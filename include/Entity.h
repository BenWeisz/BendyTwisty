#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <extern/glm/glm.hpp>

#include "Model.h"

class Entity {
   public:
    Entity(const Model* model);
    ~Entity();
    void Draw() const;
    glm::mat4 GetTransform() const;

   private:
    const Model* m_Model;
    glm::mat4 m_Translation;
    glm::mat4 m_Rotation;
    glm::mat4 m_Scale;
};