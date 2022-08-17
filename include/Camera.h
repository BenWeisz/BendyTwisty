#pragma once

#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <extern/glm/glm.hpp>
#include <extern/glm/gtc/matrix_transform.hpp>

class Camera {
   public:
    Camera(const size_t width, const size_t height, const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);
    ~Camera();
    void Move(const glm::vec3 &dir);
    glm::mat4 GetCameraMatrix() const;
    glm::vec3 GetEye() const;
    glm::vec3 GetCenter() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetRight() const;
    float GetCameraSpeed() const;
    void ProcessInput(GLFWwindow *window, const float deltaTime);
    void MouseCallback(GLFWwindow *window, double xpos, double ypos);

   private:
    glm::vec3 m_Eye;
    glm::vec3 m_Center;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::mat4 m_Proj;
    float m_CameraSpeed;
    float m_Sensitivity;
    float m_Yaw;
    float m_Pitch;
    float m_MouseLastX;
    float m_MouseLastY;
    bool m_FirstMouse;
};