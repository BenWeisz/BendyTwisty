#pragma once

#define GL_SILENCE_DEPRECATION

#include <vector>

#include "ShaderProgram.h"
#include "Entity.h"

struct ShaderEntity {
    ShaderProgram *shader;
    Entity *entity;
};

class World {
   private:
    std::vector<ShaderEntity> m_ShaderEntities;

   public:
    World();
    ~World();
    void AddShaderEntity(ShaderProgram *shader, Entity *entity);
    void Draw();
};