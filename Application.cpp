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

const size_t WIDTH = 640;
const size_t HEIGHT = 480;
const char* WINDOW_NAME = "ModelEngine";

/*
 * Callback to handle the "close window" event, once the user pressed the Escape key.
 */
static void QuitCallback(GLFWwindow* window, int key, int scancode, int action, int _mods) {
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Close the window as soon as the Escape key has been pressed
    glfwSetKeyCallback(window, QuitCallback);
    // Makes the window context current
    glfwMakeContextCurrent(window);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // Generate the model
    /*const Model* model = ModelFactory::SimplePlane();
    const Entity entity(model);

    // ShaderProgram shader("../res/base.vert", "../res/base.frag");
    glm::vec3 lightColor = glm::vec3(0.13f, 0.71f, 0.28f);
    glm::vec3 darkColor = glm::vec3(0.04f, 0.43f, 0.14f);
    glm::vec2 resolution = glm::vec2(std::gcd(WIDTH, HEIGHT), std::gcd(WIDTH, HEIGHT));

    // glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -100.f, 100.0f);
    glm::mat4 proj = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    // glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 eye = glm::vec3(0.0f, 0.0f, 1.0f);
    // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // glm::mat4 view = glm::lookAt(eye, center, up);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

    glm::mat4 mod = glm::mat4(1.0f);

    glm::mat4 mvp = proj * view * mod;*/

    // ShaderProgram shader("../res/checkered.vert", "../res/checkered.frag");
    ShaderProgram shader("../res/base.vert", "../res/base.frag");
    /*shader.Bind();
    shader.SetUniform3fv("u_LightColor", &lightColor[0]);
    shader.SetUniform3fv("u_DarkColor", &darkColor[0]);
    shader.SetUniform2fv("u_Resolution", &resolution[0]);
    shader.SetUniformMat4fv("u_MVP", mvp);
    shader.Unbind();*/

    // GLfloat data1[] = {
    //     0.0f, 0.5f, 0.0f,
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f};

    // GLuint iboData[] = {
    //     0, 1, 2};

    // Model model;
    // model.AddVertexData((GLvoid*)data1, 9, GL_FLOAT);
    // model.SetIndexData(iboData, 3);

    // std::vector<LayoutElement> layoutElements;
    // layoutElements.push_back((LayoutElement){3, GL_FLOAT});
    // model.AddBufferLayout(layoutElements);

    // model.PackModel();

    const Model* model = ModelFactory::SimplePlane();

    // Now we have a current OpenGL context, we can use OpenGL normally
    while (!glfwWindowShouldClose(window)) {
        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        model->Bind();
        model->Draw();
        model->Unbind();

        shader.Unbind();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    delete model;

    glfwTerminate();

    return 0;
}