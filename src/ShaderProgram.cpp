#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
    GLint vertexShaderID = ShaderProgram::LoadCompileShader(vertexShaderPath, GL_VERTEX_SHADER);
    if (vertexShaderID == -1)
        return;

    GLint fragmentShaderID = ShaderProgram::LoadCompileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
    if (fragmentShaderID == -1)
        return;

    m_RendererID = glCreateProgram();

    glAttachShader(m_RendererID, (GLuint)vertexShaderID);
    glAttachShader(m_RendererID, (GLuint)fragmentShaderID);

    glLinkProgram(m_RendererID);

    GLint linkStatus;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetProgramInfoLog(m_RendererID, 1024, &logLength, message);
        std::cerr << "[GLSL LINK ERROR]: " << message << std::endl;
        return;
    }
}

ShaderProgram::~ShaderProgram() {
    ShaderProgram::Unbind();

    GLuint shaderIDs[2];
    GLsizei numFound;
    glGetAttachedShaders(m_RendererID, 2, &numFound, shaderIDs);

    glDeleteShader(shaderIDs[0]);
    glDeleteShader(shaderIDs[1]);

    glDeleteProgram(m_RendererID);
}

void ShaderProgram::Bind() {
    glUseProgram(m_RendererID);
}

void ShaderProgram::Unbind() {
    glUseProgram(0);
}

GLint ShaderProgram::LoadCompileShader(const char* shaderPath, const GLenum shaderType) {
    const char* shaderSource = ShaderProgram::readShaderFile(shaderPath);

    if (shaderSource == nullptr)
        return -1;

    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    delete shaderSource;

    GLint compileFlag;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileFlag);
    if (compileFlag != GL_TRUE) {
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shaderID, 1024, &logLength, message);
        std::cerr << "[GLSL COMPILE ERROR]: FILE: " << shaderPath << " || " << message << std::endl;
        return -1;
    }

    return (GLint)shaderID;
}