/*#include "World.h"

World::World() {}

World::~World() {}

void World::AddShaderEntity(ShaderProgram *shader, Entity *entity) {
    ShaderEntity shaderEntity = {
        .shader = shader,
        .entity = entity};

    m_ShaderEntities.push_back(shaderEntity);
}

void World::Draw() {
    for (auto &shaderEntity : m_ShaderEntities) {
        shaderEntity.shader->Bind();
        shaderEntity.entity->Draw();
        shaderEntity.shader->Unbind();
    }
}*/