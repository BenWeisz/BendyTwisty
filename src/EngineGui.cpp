#include "EngineGui.h"

ImGuiIO* EngineGui::IO;
float* EngineGui::lightRGB;
float* EngineGui::planeRGB;
float* EngineGui::boxRGB;
float* EngineGui::teapotRGB;
float EngineGui::cameraSpeed;
std::vector<EntityGUIData> EngineGui::entityData;

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
    boxRGB[0] = 1.0f;
    boxRGB[1] = 0.5f;
    boxRGB[2] = 0.0f;

    teapotRGB = new float[3];
    teapotRGB[0] = 0.2f;
    teapotRGB[1] = 0.6f;
    teapotRGB[2] = 1.0f;

    cameraSpeed = 5.0f;
}

void EngineGui::Destory() {
    delete lightRGB;
    delete planeRGB;
    delete boxRGB;
    delete teapotRGB;

    for (auto data : entityData) {
        delete[] data.data;
    }
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

int EngineGui::GetEntityDataOffset(const Entity& entity, const char key) {
    if (entity.GetMetadata(ENTITY_SHOW_IN_GUI) == ENTITY_STATE_DISABLED)
        return -1;

    unsigned int offset = 0;

    if (key != ENTITY_SHOW_LIGHTING_IN_GUI) {
        if (entity.GetMetadata(ENTITY_SHOW_LIGHTING_IN_GUI) == ENTITY_STATE_ENABLED)
            offset += sizeof(bool);
    } else
        return offset;

    if (key != ENTITY_SHOW_COLOR_IN_GUI) {
        if (entity.GetMetadata(ENTITY_SHOW_COLOR_IN_GUI) == ENTITY_STATE_ENABLED)
            offset += 3 * sizeof(float);
    } else
        return offset;

    if (key != ENTITY_SHOW_TRANSFORM_IN_GUI) {
        if (entity.GetMetadata(ENTITY_SHOW_TRANSFORM_IN_GUI) == ENTITY_STATE_ENABLED)
            offset += 3 * sizeof(float);
    } else
        return offset;

    if (key != ENTITY_SHOW_ROTATION_IN_GUI || entity.GetMetadata(ENTITY_SHOW_ROTATION_IN_GUI) != ENTITY_STATE_ENABLED) {
        return -1;
    } else
        return offset;
}

unsigned int EngineGui::GetEntityGUIDataSize(const Entity& entity) {
    if (entity.GetMetadata(ENTITY_SHOW_IN_GUI) == ENTITY_STATE_DISABLED)
        return 0;

    unsigned int size = 0;
    if (entity.GetMetadata(ENTITY_SHOW_LIGHTING_IN_GUI) == ENTITY_STATE_ENABLED)
        size += sizeof(bool);
    if (entity.GetMetadata(ENTITY_SHOW_COLOR_IN_GUI) == ENTITY_STATE_ENABLED)
        size += 3 * sizeof(float);
    if (entity.GetMetadata(ENTITY_SHOW_TRANSFORM_IN_GUI) == ENTITY_STATE_ENABLED)
        size += 3 * sizeof(float);
    if (entity.GetMetadata(ENTITY_SHOW_ROTATION_IN_GUI) == ENTITY_STATE_ENABLED)
        size += 3 * sizeof(float);

    return size;
}

void EngineGui::RegisterEntity(Entity& entity) {
    if (entity.GetMetadata(ENTITY_SHOW_IN_GUI) != ENTITY_STATE_ENABLED)
        return;

    unsigned int size = GetEntityGUIDataSize(entity);
    char* dataBytes = new char[size];
    for (int i = 0; i < size; i++)
        dataBytes[i] = 0x00;

    EntityGUIData data = {
        .entity = entity,
        .data = dataBytes,
        .size = size};

    entityData.push_back(data);
}

void EngineGui::ShowSettingsMenu(const Light& light, const Camera& camera) {
    for (auto data : entityData) {
        Entity& entity = data.entity;
        char* settingsData = data.data;
        if (ImGui::CollapsingHeader(entity.GetName().c_str())) {
            if (entity.GetMetadata(ENTITY_SHOW_LIGHTING_IN_GUI) == ENTITY_STATE_ENABLED) {
                bool* handle = (bool*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_LIGHTING_IN_GUI));
                ImGui::Checkbox(("Use Lighting##" + entity.GetName()).c_str(), handle);
            }
            if (entity.GetMetadata(ENTITY_SHOW_COLOR_IN_GUI) == ENTITY_STATE_ENABLED) {
                float* handle = (float*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_COLOR_IN_GUI));
                ImGui::ColorEdit3(("Flat Color##" + entity.GetName()).c_str(), handle);
            }
            if (entity.GetMetadata(ENTITY_SHOW_TRANSFORM_IN_GUI) == ENTITY_STATE_ENABLED) {
                float* handle = (float*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_TRANSFORM_IN_GUI));
                ImGui::InputFloat3(("Transform##" + entity.GetName()).c_str(), handle);
            }
            if (entity.GetMetadata(ENTITY_SHOW_ROTATION_IN_GUI) == ENTITY_STATE_ENABLED) {
                float* handle = (float*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_ROTATION_IN_GUI));
                ImGui::InputFloat3(("Rotation##" + entity.GetName()).c_str(), handle);
            }
        }
    }
}

// void EngineGui::ShowSettingsMenu(const Light& light, const Camera& camera) {
//     ImGui::ColorEdit3("Light Color", (float*)lightRGB);
//     pointLight->SetColor(lightRGB);

//     ImGui::ColorEdit3("Plane Color", (float*)planeRGB);
//     plane->SetColor(planeRGB);

//     ImGui::ColorEdit3("Box Color", (float*)boxRGB);
//     box->SetColor(boxRGB);

//     ImGui::ColorEdit3("Teapot", (float*)teapotRGB);
//     teapot->SetColor(teapotRGB);

//     ImGui::SetNextItemWidth(220);
//     ImGui::DragFloat("Camera Move Speed", &cameraSpeed, 5.0f);
//     camera->SetCameraSpeed(cameraSpeed);

//     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
// }

bool EngineGui::HasContent() {
    return entityData.size();
}