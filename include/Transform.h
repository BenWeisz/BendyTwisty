#pragma once

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Transform {
   public:
    Transform();
    glm::mat4 GetMatrix() const;
    void SetScale(const float scaleX, const float scaleY, const float scaleZ);
    void SetRotation(const float rotationX, const float rotationY, const float rotationZ);
    void SetTranslation(const float translationX, const float translationY, const float translationZ);
    glm::vec3 GetTranslation() const;

   private:
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;
    glm::vec3 m_Translation;
};
