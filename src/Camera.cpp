#include "Camera.h"

#include <iostream>

Camera::Camera(const size_t width, const size_t height, const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) : m_Eye(eye), m_Center(center), m_Up(up) {
    m_Proj = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

Camera::~Camera() {}

void Camera::Move(const glm::vec3 &dir) {
    m_Eye += dir;
}

glm::mat4 Camera::GetCameraMatrix() const {
    glm::mat4 viewMatrix = glm::lookAt(m_Eye, m_Center, m_Up);
    return m_Proj * viewMatrix;
}