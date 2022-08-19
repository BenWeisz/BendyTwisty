#pragma once

#include <extern/glm/glm.hpp>
#include <extern/glm/glm.hpp>
#include <extern/glm/gtc/matrix_transform.hpp>
#include <extern/glm/gtx/transform.hpp>

class Transform {
   public:
    Transform();
    glm::mat4 GetMatrix() const;
    void SetScale(const float scaleX, const float scaleY, const float scaleZ);
    void SetRotation(const float rotationX, const float rotationY, const float rotationZ);
    void SetTranslation(const float translationX, const float translationY, const float translationZ);

   private:
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;
    glm::vec3 m_Translation;
};
