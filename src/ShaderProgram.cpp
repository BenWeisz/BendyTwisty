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

void ShaderProgram::Bind() const {
    glUseProgram(m_RendererID);
}

void ShaderProgram::Unbind() const {
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

void ShaderProgram::SetUniformMat4fv(const char* uniformName, const glm::mat4& mat) {
    GLint uniformID = ShaderProgram::GetUniformLocation(uniformName);
    if (uniformID == -1)
        return;

    glUniformMatrix4fv(uniformID, 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetUniform3fv(const char* uniformName, const GLfloat* val) {
    GLint uniformID = ShaderProgram::GetUniformLocation(uniformName);
    if (uniformID == -1)
        return;

    glUniform3fv(uniformID, 1, val);
}

void ShaderProgram::SetUniform2fv(const char* uniformName, const GLfloat* val) {
    GLint uniformID = ShaderProgram::GetUniformLocation(uniformName);
    if (uniformID == -1)
        return;

    glUniform2fv(uniformID, 1, val);
}

const GLint ShaderProgram::GetUniformLocation(const char* uniformName) const {
    GLint uniformID = glGetUniformLocation(m_RendererID, uniformName);
    if (uniformID == -1) {
        std::cerr << "[ERROR]: Cannot find uniform of name: " << uniformName << std::endl;
        return -1;
    }
    return uniformID;
}