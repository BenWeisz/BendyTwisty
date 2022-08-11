#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

class IndexBuffer {
   public:
    IndexBuffer(const GLvoid *data, const GLsizeiptr size);
    ~IndexBuffer();
    void Bind() const;
    void Unbind() const;

   private:
    GLuint m_RendererID;
};