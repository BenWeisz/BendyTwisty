#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <numeric>

#include "ShaderProgram.h"
#include "Model.h"
#include "ModelLoader.h"
#include "Entity.h"
#include "ModelRenderer.h"
#include "EngineGui.h"

#include "PointLight.h"
#include "DirectionalLight.h"
#include "Material.h"

#include "custom/Plane.h"
#include "custom/rod/Rod.h"

#if __DEBUG__
#define RESOURCE_DIR(x) "./res/shaders/" x
#else
#define RESOURCE_DIR(x) "../res/shaders/" x
#endif

const size_t WIDTH = 640;
const size_t HEIGHT = 480;
const char* WINDOW_NAME = "ModelEngine";

static void ExitCallback(GLFWwindow* window, int key, int scancode, int action, int _mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void) {
    GLFWwindow* window;

    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return -1;  // Initialize the lib
    }

    // Minimum target is OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Close the window as soon as the Escape key has been pressed
    glfwSetKeyCallback(window, ExitCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Makes the window context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Set up ImGui
    EngineGui::Init(window);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    ModelRenderer modelRenderer(window, WIDTH, HEIGHT);

    Plane plane;
    Rod rod(10.0f, 20);

    // Directional Light
    ShaderProgram phongDirectionalFnShader(RESOURCE_DIR("normals_f.vert"), RESOURCE_DIR("light_shading_directional_fn.frag"));
    PhongMaterial* phongDirectionalFnMaterial_Plane = new PhongMaterial(
        phongDirectionalFnShader,
        MATERIAL_PHONG,
        glm::vec3(0.498f, 0.588f, 0.6f),
        0.1f,
        1.0f,
        0.5f);

    // Point Light
    // ShaderProgram phongPointFnShader(RESOURCE_DIR("normals_f.vert"), RESOURCE_DIR("light_shading_point_fn.frag"));
    // PhongMaterial* phongPointFnMaterial_Plane = new PhongMaterial(
    //     phongPointFnShader,
    //     MATERIAL_PHONG,
    //     glm::vec3(0.498f, 0.588f, 0.6f),
    //     0.1f,
    //     1.0f,
    //     0.5f);

    ShaderProgram flatNormalShader(RESOURCE_DIR("normal_bypass.vert"), RESOURCE_DIR("flat.frag"));
    FlatMaterial* flatMaterial_Plane = new FlatMaterial(
        flatNormalShader,
        MATERIAL_FLAT,
        glm::vec3(0.498f, 0.588f, 0.6f));

    ShaderProgram flatShader(RESOURCE_DIR("base.vert"), RESOURCE_DIR("flat.frag"));
    FlatMaterial* flatMaterial_Rod = new FlatMaterial(
        flatShader,
        MATERIAL_FLAT,
        glm::vec3(1.0f, 1.0f, 1.0f));

    plane.AddMaterial(flatMaterial_Plane);
    plane.AddMaterial(phongDirectionalFnMaterial_Plane);
    plane.SetIsLightingEnabled(true);

    rod.AddMaterial(flatMaterial_Rod);
    rod.SetIsLightingEnabled(false);

    // PointLight pointLight(&flatNormalShader, glm::vec3(1.0f, 1.0f, 1.0f), "Rotating Light");
    // modelRenderer.AddLight(&pointLight);

    DirectionalLight directionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.2f, 1.5f), "Sun");
    modelRenderer.AddLight(&directionalLight);

    // modelRenderer.AddEntity(&plane);
    modelRenderer.AddEntity(&rod);

    float deltaTime = 0.0f;
    float lastTime = (float)glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    glClearColor(15.0f / 255.0f, 44.0f / 255.0f, 92.0f / 255.0f, 1.0f);

    EngineGui::RegisterEntity(plane);
    EngineGui::RegisterEntity(rod);

    // EngineGui::RegisterLight(&pointLight);
    EngineGui::RegisterLight(&directionalLight);

    // Now we have a current OpenGL context, we can use OpenGL normally
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Poll for and process events
        glfwPollEvents();
        modelRenderer.Update(deltaTime);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelRenderer.Draw(deltaTime);

        if (EngineGui::HasContent() && false) {
            EngineGui::StartDraw("ModelEngine Settings");
            EngineGui::ShowSettingsMenu(modelRenderer.GetCamera());
            EngineGui::EndDraw();
        }

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    delete phongDirectionalFnMaterial_Plane;
    // delete phongPointFnMaterial_Plane;
    delete flatMaterial_Plane;
    delete flatMaterial_Rod;

    EngineGui::Destory();
    glfwTerminate();

    return 0;
}