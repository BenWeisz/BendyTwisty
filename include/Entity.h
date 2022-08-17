#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <extern/glm/glm.hpp>
#include <extern/glm/gtc/matrix_transform.hpp>
#include <extern/glm/gtx/transform.hpp>

#include "Model.h"

class Entity {
   public:
    Entity();
    ~Entity();
    void Draw(const float deltaTime) const;
    void Update(const float deltaTime);
    glm::mat4 GetTransform() const;

   protected:
    Model* m_Model;
    glm::vec3 m_Translation;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
};