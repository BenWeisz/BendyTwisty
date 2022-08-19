#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include "Model.h"
#include "Transform.h"
#include "Light.h"

class Entity {
   public:
    Entity();
    ~Entity();
    void Draw(const float deltaTime) const;
    void Draw(const float deltaTime, Light* const light) const;
    void Update(const float deltaTime);
    Transform GetTransform() const;
    bool IsLightingEnabled() const;

   protected:
    Model* m_Model;
    Transform m_Transform;
    bool m_LightingEnabled;
};