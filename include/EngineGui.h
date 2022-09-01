#pragma once

#include <vector>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "Light.h"
#include "Camera.h"
#include "Entity.h"

struct EntityGUIData {
    Entity& entity;
    char* data;
    unsigned int size;
};

namespace EngineGui {
extern ImGuiIO* IO;
extern float* lightRGB;
extern float* planeRGB;
extern float* boxRGB;
extern float* teapotRGB;
extern float cameraSpeed;
extern std::vector<EntityGUIData> entityData;
extern void Init(GLFWwindow* window);
void StartDraw(const char* title);
void EndDraw();
int GetEntityDataOffset(const Entity& entity, const char key);
unsigned int GetEntityGUIDataSize(const Entity& entity);
void RegisterEntity(Entity& entity);
void ShowSettingsMenu(const Light& light, const Camera& camera);
bool HasContent();
void Destory();
};  // namespace EngineGui