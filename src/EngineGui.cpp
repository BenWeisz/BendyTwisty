#include "EngineGui.h"

ImGuiIO* EngineGui::IO;
float* EngineGui::lightRGB;
float* EngineGui::planeRGB;
float* EngineGui::boxRGB;
float EngineGui::cameraSpeed;

void EngineGui::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    IO = &ImGui::GetIO();

    IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();
    if (IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    lightRGB = new float[3];
    lightRGB[0] = 1.0f;
    lightRGB[1] = 1.0f;
    lightRGB[2] = 1.0f;

    planeRGB = new float[3];
    planeRGB[0] = 0.498f;
    planeRGB[1] = 0.588f;
    planeRGB[2] = 0.6f;

    boxRGB = new float[3];
    boxRGB[0] = 0.498f;
    boxRGB[1] = 0.588f;
    boxRGB[2] = 0.6f;

    cameraSpeed = 5.0f;
}

void EngineGui::Destory() {
    delete lightRGB;
}

void EngineGui::StartDraw(const char* title) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin(title);
}

void EngineGui::EndDraw() {
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void EngineGui::ShowSettingsMenu(Light* const light, Plane* const plane, Box* const box, Camera* const camera) {
    ImGui::ColorEdit3("Light Color", (float*)lightRGB);
    light->SetColor(lightRGB);

    ImGui::ColorEdit3("Plane Color", (float*)planeRGB);
    plane->SetColor(planeRGB);

    ImGui::ColorEdit3("Box Color", (float*)boxRGB);
    box->SetColor(boxRGB);

    ImGui::SetNextItemWidth(220);
    ImGui::DragFloat("Camera Move Speed", &cameraSpeed, 5.0f);
    camera->SetCameraSpeed(cameraSpeed);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}