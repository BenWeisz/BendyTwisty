#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
   private:
    GLuint m_RendererID;

   public:
    VertexArray();
    ~VertexArray();
    void Bind() const;
    void Unbind() const;
    void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
};