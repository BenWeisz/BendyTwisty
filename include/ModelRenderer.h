#pragma once

#include <GLFW/glfw3.h>
#include <extern/glm/glm.hpp>

#include <vector>

#include "Entity.h"
#include "ShaderProgram.h"
#include "Camera.h"

struct EntityShader {
    Entity* entity;
    ShaderProgram* shader;
};

class ModelRenderer {
   public:
    ModelRenderer(GLFWwindow* window, const size_t width, const size_t height);
    ~ModelRenderer();
    void Draw(const float deltaTime);
    void Update(const float deltaTime);
    void AddEntityShaderPair(Entity* entity, ShaderProgram* shader);

   private:
    GLFWwindow* m_Window;
    Camera* m_Camera;
    std::vector<EntityShader> m_EntityShaderPairs;
};