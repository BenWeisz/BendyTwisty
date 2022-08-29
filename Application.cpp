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
#include "PointLight.h"
#include "EngineGui.h"

#include "custom/RainbowBox.h"
#include "custom/Plane.h"
#include "custom/Box.h"
#include "custom/Teapot.h"

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

    RainbowBox rainbowBox;
    Plane plane;
    Box box;
    Teapot teapot;

    ShaderProgram rainboxShader("../res/shaders/rainbow.vert", "../res/shaders/rainbow.frag");
    ShaderProgram flatShader("../res/shaders/base.vert", "../res/shaders/flat.frag");
    ShaderProgram flatPhongShader("../res/shaders/normals.vert", "../res/shaders/flatPhong.frag");
    ShaderProgram lightShader("../res/shaders/base.vert", "../res/shaders/base.frag");
    ShaderProgram interpPhongShader("../res/shaders/interp_norm.vert", "../res/shaders/interp_norm.frag");

    PointLight pointLight(&flatShader, glm::vec3(1.0f, 1.0f, 1.0f));
    modelRenderer.SetPointLight(&pointLight);

    // modelRenderer.AddEntityShaderPair(&rainbowBox, &rainboxShader);
    modelRenderer.AddEntityShaderPair(&plane, &flatPhongShader);
    modelRenderer.AddEntityShaderPair(&box, &flatPhongShader);
    modelRenderer.AddEntityShaderPair(&teapot, &interpPhongShader);

    float deltaTime = 0.0f;
    float lastTime = (float)glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    glClearColor(0.0f, 0.4f, 0.8f, 1.0f);

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

        EngineGui::StartDraw("ModelEngine Settings");
        EngineGui::ShowSettingsMenu(&pointLight, &plane, &box, &teapot, modelRenderer.GetCamera());
        EngineGui::EndDraw();

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    EngineGui::Destory();
    glfwTerminate();

    return 0;
}