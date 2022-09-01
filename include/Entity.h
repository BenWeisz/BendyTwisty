#pragma once

#define GL_SILENCE_DEPRECATION

#include <unordered_map>
#include <string>

#include <OpenGL/gl3.h>

#include "Model.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"

#define ENTITY_SHOW_IN_GUI 0x00
#define ENTITY_SHOW_LIGHTING_IN_GUI 0x01
#define ENTITY_SHOW_COLOR_IN_GUI 0x02
#define ENTITY_SHOW_TRANSFORM_IN_GUI 0x03
#define ENTITY_SHOW_ROTATION_IN_GUI 0x04

#define ENTITY_STATE_ENABLED 0x00
#define ENTITY_STATE_DISABLED 0x01

class Entity {
   public:
    Entity(const std::string& name);
    ~Entity();
    virtual void Draw(const float deltaTime, ShaderProgram* const shader) const;
    virtual void Draw(const float deltaTime, ShaderProgram* const shader, Light* const light, Camera* const camera) const;
    virtual void Update(const float deltaTime);
    const Transform& GetTransform() const;
    char GetMetadata(const char key) const;
    const std::string& GetName() const;
    bool GetIsLightingEnabled() const;

   protected:
    static int m_EntityNum;
    std::string m_Name;
    Model* m_Model;
    Transform m_Transform;
    std::unordered_map<char, char> m_Metadata;
    bool m_IsLightingEnabled;
};