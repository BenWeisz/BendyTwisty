#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include "Light.h"
#include "Camera.h"
#include "custom/Plane.h"
#include "custom/Box.h"
#include "custom/Teapot.h"

namespace EngineGui {
extern ImGuiIO* IO;
extern float* lightRGB;
extern float* planeRGB;
extern float* boxRGB;
extern float* teapotRGB;
extern float cameraSpeed;
void Init(GLFWwindow* window);
void StartDraw(const char* title);
void EndDraw();
void ShowSettingsMenu(Light* const light, Plane* const plane, Box* const box, Teapot* const teapot, Camera* const camera);
void Destory();
};