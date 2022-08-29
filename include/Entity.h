#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include "Model.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"

class Entity {
   public:
    Entity();
    ~Entity();
    virtual void Draw(const float deltaTime, ShaderProgram* const shader) const;
    virtual void Draw(const float deltaTime, ShaderProgram* const shader, Light* const light, Camera* const camera) const;
    virtual void Update(const float deltaTime);
    Transform GetTransform() const;
    bool IsLightingEnabled() const;

   protected:
    Model* m_Model;
    Transform m_Transform;
    bool m_LightingEnabled;
};