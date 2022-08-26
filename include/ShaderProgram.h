#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string>

class ShaderProgram {
   public:
    ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~ShaderProgram();
    void Bind() const;
    void Unbind() const;
    GLint LoadCompileShader(const std::string& shaderPath, const GLenum shaderType);
    void SetUniformMat4fv(const std::string& uniformName, const glm::mat4& mat);
    void SetUniform3fv(const std::string& uniformName, const GLfloat* val);
    void SetUniform2fv(const std::string& uniformName, const GLfloat* val);
    void SetUniform1f(const std::string& uniformName, const GLfloat val);

   private:
    GLuint m_RendererID;
    std::unordered_map<std::string, GLint> m_UniformCache;
    static const char* readShaderFile(const std::string& path) {
        // Open the file
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Error: Could not load shader file: " << path << std::endl;
            return nullptr;
        }

        // Get the file size
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();

        // Read the file
        char* source = new char[fileSize + 1];
        file.seekg(0, std::ios::beg);
        file.read(source, fileSize);

        source[fileSize] = '\0';

        return source;
    }
    const GLint GetUniformLocation(const std::string& uniformName);
};