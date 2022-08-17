#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <extern/glm/glm.hpp>
#include <extern/glm/gtc/matrix_transform.hpp>
#endif

#include <iostream>
#include <numeric>

#include "ShaderProgram.h"
#include "Model.h"
#include "ModelFactory.h"
#include "Entity.h"
#include "World.h"
#include "WorldFactory.h"
#include "Camera.h"

#include "custom/Box.h"

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

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glm::vec3 initialEye = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 initialCenter = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 initialUp = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera camera = Camera(WIDTH, HEIGHT, initialEye, initialCenter, initialUp);

    Box box;
    glm::mat4 u_MVP = camera.GetCameraMatrix() * box.GetTransform();

    ShaderProgram shader("../res/colorcube.vert", "../res/colorcube.frag");
    shader.Bind();
    shader.SetUniformMat4fv("u_MVP", u_MVP);
    shader.Unbind();

    float deltaTime = 0.0f;
    float lastTime = (float)glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    // Now we have a current OpenGL context, we can use OpenGL normally
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        camera.ProcessInput(window, deltaTime);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();
        u_MVP = camera.GetCameraMatrix() * box.GetTransform();
        shader.SetUniformMat4fv("u_MVP", u_MVP);

        box.Update(deltaTime);
        box.Draw(deltaTime);

        shader.Unbind();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}