#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <cassert>
#include <vector>
#include <string>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

class Model {
   public:
    Model();
    ~Model();
    void Bind() const;
    void Unbind() const;
    void AddVertexData(const GLvoid *data, const unsigned int count, const GLenum type);
    void SetIndexData(const GLuint *data, const unsigned int count);
    void AddBufferLayout(const std::vector<LayoutElement> layoutElements);
    void PackModel();
    void Draw() const;
    void SetPrimitive(const GLenum type, const GLfloat size);

   private:
    std::vector<VertexBuffer> m_VBOs;
    IndexBuffer m_IBO;
    VertexArray m_VAO;
    std::vector<VertexBufferLayout> m_BufferLayouts;
    GLenum m_PrimitiveType;
};