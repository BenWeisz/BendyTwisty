#include "Camera.h"

Camera::Camera(const size_t width, const size_t height, const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) : m_Eye(eye), m_CameraSpeed(5.0f), m_Sensitivity(0.1f), m_Yaw(-90.0f), m_Pitch(0.0f), m_FirstMouse(true) {
    m_Center = glm::normalize(center);
    m_Up = glm::normalize(up);
    m_Right = glm::normalize(glm::cross(m_Center, m_Up));

    m_MouseLastX = width / 2;
    m_MouseLastY = height / 2;

    m_Proj = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

Camera::~Camera() {}

void Camera::Move(const glm::vec3 &dir) {
    m_Eye += dir;
}

glm::mat4 Camera::GetCameraMatrix() const {
    glm::mat4 viewMatrix = glm::lookAt(m_Eye, m_Eye + m_Center, m_Up);
    return m_Proj * viewMatrix;
}

glm::vec3 Camera::GetEye() const {
    return m_Eye;
}

glm::vec3 Camera::GetCenter() const {
    return m_Center;
}

glm::vec3 Camera::GetUp() const {
    return m_Up;
}

glm::vec3 Camera::GetRight() const {
    return m_Right;
}

float Camera::GetCameraSpeed() const {
    return m_CameraSpeed;
}

void Camera::ProcessInput(GLFWwindow *window, const float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Move(deltaTime * m_CameraSpeed * m_Center);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Move(deltaTime * m_CameraSpeed * -m_Center);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Move(deltaTime * m_CameraSpeed * -m_Right);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Move(deltaTime * m_CameraSpeed * m_Right);

    double xpos;
    double ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (m_FirstMouse) {
        m_MouseLastX = xpos;
        m_MouseLastY = ypos;
        m_FirstMouse = false;
    }

    float xOffset = xpos - m_MouseLastX;
    float yOffset = m_MouseLastY - ypos;
    m_MouseLastX = xpos;
    m_MouseLastY = ypos;

    xOffset *= m_Sensitivity;
    yOffset *= m_Sensitivity;

    m_Yaw += xOffset;
    m_Pitch += yOffset;

    if (m_Pitch > 44.0f)
        m_Pitch = 44.0f;
    if (m_Pitch < -44.0f)
        m_Pitch = -44.0f;

    glm::vec3 dir;
    dir.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    dir.y = sin(glm::radians(m_Pitch));
    dir.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Center = glm::normalize(dir);

    dir.y = sin(glm::radians(m_Pitch + 90.0f));
    m_Up = glm::normalize(dir);

    m_Right = glm::normalize(glm::cross(m_Center, m_Up));
}