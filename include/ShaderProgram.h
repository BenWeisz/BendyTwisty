#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <fstream>
#include <iostream>

class ShaderProgram {
   public:
    ShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
    ~ShaderProgram();
    void Bind();
    void Unbind();
    GLint LoadCompileShader(const char *shaderPath, const GLenum shaderType);

   private:
    GLuint m_RendererID;
    static const char *readShaderFile(const char *path) {
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
        char *source = new char[fileSize + 1];
        file.seekg(0, std::ios::beg);
        file.read(source, fileSize);

        source[fileSize] = '\0';

        return source;
    }
};