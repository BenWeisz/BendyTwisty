#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "Entity.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "PointLight.h"

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
    void SetPointLight(PointLight* pointLight);
    Camera* GetCamera() const;

   private:
    GLFWwindow* m_Window;
    Camera* m_Camera;
    PointLight* m_PointLight;
    std::vector<EntityShader> m_EntityShaderPairs;
};