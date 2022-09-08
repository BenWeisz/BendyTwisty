#pragma once

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

class VertexBuffer {
   public:
    VertexBuffer();
    VertexBuffer(const GLvoid *data, const GLsizeiptr size, const GLenum usage);
    ~VertexBuffer();
    void SetData(const GLvoid *data, const GLsizeiptr size, const GLenum usage);
    void Bind() const;
    void Unbind() const;

   private:
    GLuint m_RendererID;
};