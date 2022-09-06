#include "EngineGui.h"

ImGuiIO* EngineGui::IO;
std::vector<EntityGUIData> EngineGui::entityData;
Light* EngineGui::light = nullptr;

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
}

void EngineGui::Destory() {
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

    if (key != ENTITY_SHOW_TRANSLATION_IN_GUI) {
        if (entity.GetMetadata(ENTITY_SHOW_TRANSLATION_IN_GUI) == ENTITY_STATE_ENABLED)
            offset += 3 * sizeof(float);
    } else
        return offset;

    if (key != ENTITY_SHOW_ROTATION_IN_GUI) {
        if (entity.GetMetadata(ENTITY_SHOW_ROTATION_IN_GUI) == ENTITY_STATE_ENABLED)
            offset += 3 * sizeof(float);
    } else
        return offset;

    if (key != ENTITY_SHOW_SCALE_IN_GUI || entity.GetMetadata(ENTITY_SHOW_SCALE_IN_GUI) != ENTITY_STATE_ENABLED) {
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
    if (entity.GetMetadata(ENTITY_SHOW_TRANSLATION_IN_GUI) == ENTITY_STATE_ENABLED)
        size += 3 * sizeof(float);
    if (entity.GetMetadata(ENTITY_SHOW_ROTATION_IN_GUI) == ENTITY_STATE_ENABLED)
        size += 3 * sizeof(float);
    if (entity.GetMetadata(ENTITY_SHOW_SCALE_IN_GUI) == ENTITY_STATE_ENABLED)
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

    LoadSettings(entity, dataBytes);

    EntityGUIData data = {
        .entity = entity,
        .data = dataBytes,
        .size = size};

    entityData.push_back(data);
}

void EngineGui::RegisterLight(Light* newLight) {
    light = newLight;
}

void EngineGui::ShowSettingsMenu(const Camera& camera) {
    if (ImGui::CollapsingHeader("Entities")) {
        for (auto data : entityData) {
            Entity& entity = data.entity;
            char* settingsData = data.data;
            if (ImGui::CollapsingHeader(entity.GetName().c_str())) {
                if (entity.GetMetadata(ENTITY_SHOW_LIGHTING_IN_GUI) == ENTITY_STATE_ENABLED) {
                    bool* handle = (bool*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_LIGHTING_IN_GUI));
                    ImGui::Checkbox(("Use Lighting##" + entity.GetName()).c_str(), handle);
                    entity.SetIsLightingEnabled(*handle);
                }
                if (entity.GetMetadata(ENTITY_SHOW_COLOR_IN_GUI) == ENTITY_STATE_ENABLED) {
                    float* handle = (float*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_COLOR_IN_GUI));
                    ImGui::ColorEdit3(("Flat Color##" + entity.GetName()).c_str(), handle);

                    FlatMaterial* flatMaterial = (FlatMaterial*)entity.GetActiveMaterial();
                    flatMaterial->color[0] = handle[0];
                    flatMaterial->color[1] = handle[1];
                    flatMaterial->color[2] = handle[2];
                }
                if (entity.GetMetadata(ENTITY_SHOW_TRANSLATION_IN_GUI) == ENTITY_STATE_ENABLED) {
                    float* handle = (float*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_TRANSLATION_IN_GUI));
                    ImGui::InputFloat3(("Transform##" + entity.GetName()).c_str(), handle);

                    Transform& transform = entity.GetTransform();
                    transform.SetTranslation(handle[0], handle[1], handle[2]);
                }
                if (entity.GetMetadata(ENTITY_SHOW_ROTATION_IN_GUI) == ENTITY_STATE_ENABLED) {
                    float* handle = (float*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_ROTATION_IN_GUI));
                    ImGui::InputFloat3(("Rotation##" + entity.GetName()).c_str(), handle);

                    Transform& transform = entity.GetTransform();
                    transform.SetRotation(handle[0], handle[1], handle[2]);
                }
                if (entity.GetMetadata(ENTITY_SHOW_SCALE_IN_GUI) == ENTITY_STATE_ENABLED) {
                    float* handle = (float*)(settingsData + EngineGui::GetEntityDataOffset(entity, ENTITY_SHOW_SCALE_IN_GUI));
                    ImGui::InputFloat3(("Scale##" + entity.GetName()).c_str(), handle);

                    Transform& transform = entity.GetTransform();
                    transform.SetScale(handle[0], handle[1], handle[2]);
                }
            }
        }
    }
    if (ImGui::CollapsingHeader("Light") && light != nullptr) {
        float color[3];
        auto colorVec = light->GetColor();
        color[0] = colorVec.x;
        color[1] = colorVec.y;
        color[2] = colorVec.z;

        ImGui::ColorEdit3("Color", color);
        light->SetColor(color);
    }
    ImGui::SetNextItemWidth(350);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

bool EngineGui::HasContent() {
    return entityData.size();
}

void EngineGui::LoadSettings(Entity& entity, char* data) {
    if (entity.GetMetadata(ENTITY_SHOW_IN_GUI) == ENTITY_STATE_DISABLED)
        return;

    if (entity.GetMetadata(ENTITY_SHOW_LIGHTING_IN_GUI) == ENTITY_STATE_ENABLED) {
        bool* isLightingEnabled = (bool*)data;
        *isLightingEnabled = entity.GetIsLightingEnabled();
    }

    if (entity.GetMetadata(ENTITY_SHOW_COLOR_IN_GUI) == ENTITY_STATE_ENABLED) {
        FlatMaterial* flatMaterial = (FlatMaterial*)entity.GetActiveMaterial();
        float* settingColor = (float*)(data + GetEntityDataOffset(entity, ENTITY_SHOW_COLOR_IN_GUI));
        settingColor[0] = flatMaterial->color[0];
        settingColor[1] = flatMaterial->color[1];
        settingColor[2] = flatMaterial->color[2];
    }

    if (entity.GetMetadata(ENTITY_SHOW_TRANSLATION_IN_GUI) == ENTITY_STATE_ENABLED) {
        Transform& transform = entity.GetTransform();
        float* settingTranslation = (float*)(data + GetEntityDataOffset(entity, ENTITY_SHOW_TRANSLATION_IN_GUI));

        auto translation = transform.GetTranslation();
        settingTranslation[0] = translation[0];
        settingTranslation[1] = translation[1];
        settingTranslation[2] = translation[2];
    }

    if (entity.GetMetadata(ENTITY_SHOW_ROTATION_IN_GUI) == ENTITY_STATE_ENABLED) {
        Transform& transform = entity.GetTransform();
        float* settingRotation = (float*)(data + GetEntityDataOffset(entity, ENTITY_SHOW_ROTATION_IN_GUI));

        auto rotation = transform.GetRotation();
        settingRotation[0] = rotation[0];
        settingRotation[1] = rotation[1];
        settingRotation[2] = rotation[2];
    }

    if (entity.GetMetadata(ENTITY_SHOW_SCALE_IN_GUI) == ENTITY_STATE_ENABLED) {
        Transform& transform = entity.GetTransform();
        float* settingScale = (float*)(data + GetEntityDataOffset(entity, ENTITY_SHOW_SCALE_IN_GUI));

        auto scale = transform.GetScale();
        settingScale[0] = scale[0];
        settingScale[1] = scale[1];
        settingScale[2] = scale[2];
    }
}