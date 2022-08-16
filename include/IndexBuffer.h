#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

class IndexBuffer {
   public:
    IndexBuffer();
    IndexBuffer(const GLvoid *data, const GLsizei count);
    ~IndexBuffer();
    void SetData(const GLvoid *data, const GLsizei count);
    void Bind() const;
    void Unbind() const;
    GLsizei GetIndexCount() const;

   private:
    GLuint m_RendererID;
    GLsizei m_IndexCount;
};