#include "Transform.h"

Transform::Transform() {
    m_Translation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::mat4 Transform::GetMatrix() const {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, m_Translation);

    transform = glm::rotate(transform, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, m_Rotation.y, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::rotate(transform, m_Rotation.z, glm::vec3(1.0f, 0.0f, 0.0f));

    transform = glm::scale(transform, m_Scale);

    return transform;
}

void Transform::SetScale(const float scaleX, const float scaleY, const float scaleZ) {
    m_Scale = glm::vec3(scaleX, scaleY, scaleZ);
}

void Transform::SetRotation(const float rotationX, const float rotationY, const float rotationZ) {
    m_Rotation = glm::vec3(rotationX * (M_PI / 180.0f), rotationY * (M_PI / 180.0f), rotationZ * (M_PI / 180.0f));
}

void Transform::SetTranslation(const float translationX, const float translationY, const float translationZ) {
    m_Translation = glm::vec3(translationX, translationY, translationZ);
}

glm::vec3 Transform::GetTranslation() const {
    return m_Translation;
}