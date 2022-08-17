#pragma once

#define GL_SILENCE_DEPRECATION

#include <extern/glm/glm.hpp>
#include <extern/glm/gtc/matrix_transform.hpp>

class Camera {
   public:
    Camera(const size_t width, const size_t height, const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);
    ~Camera();
    void Move(const glm::vec3 &dir);
    glm::mat4 GetCameraMatrix() const;

   private:
    glm::vec3 m_Eye;
    glm::vec3 m_Center;
    glm::vec3 m_Up;
    glm::mat4 m_Proj;
};