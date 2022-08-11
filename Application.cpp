#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <extern/glm/glm.hpp>
#include <extern/glm/gtc/matrix_transform.hpp>
#endif

#include <iostream>

#include "ShaderProgram.h"
#include "Model.h"
#include "ModelFactory.h"
#include "Entity.h"

const size_t WIDTH = 640;
const size_t HEIGHT = 480;
const char* WINDOW_NAME = "ModelEngine";

/*
 * Callback to handle the "close window" event, once the user pressed the Escape key.
 */
static void quit_callback(GLFWwindow* window, int key, int scancode, int action, int _mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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
    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Close the window as soon as the Escape key has been pressed
    glfwSetKeyCallback(window, quit_callback);
    // Makes the window context current
    glfwMakeContextCurrent(window);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // Generate the model
    const Model* model = ModelFactory::CheckeredPlane(1.0f, 1.0f, 2, 2);
    const Entity entity(model);

    ShaderProgram shader("../res/base.vert", "../res/base.frag");

    // glm::mat4 u_Test = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f);
    // glm::mat4 u_Test = glm::mat4(1.0f);
    // shader.Bind();
    // shader.SetUniformMat4f("u_Test", u_Test);
    // shader.Unbind();

    // const char* shaderSource = ShaderProgram::readShaderFile("../res/base.vert");

    // Now we have a current OpenGL context, we can use OpenGL normally
    while (!glfwWindowShouldClose(window)) {
        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();
        // vao.Bind();
        // ibo.Bind();

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid*)0);

        // ibo.Unbind();
        // vao.Unbind();

        entity.Draw();

        shader.Unbind();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}