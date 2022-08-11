#pragma once

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

class VertexBuffer {
   public:
    VertexBuffer(const GLvoid *data, const GLsizeiptr size);
    ~VertexBuffer();
    void Bind() const;
    void Unbind() const;

   private:
    GLuint m_RendererID;
};