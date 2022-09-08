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

#define MODEL_STATIC GL_STATIC_DRAW
#define MODEL_STREAMING GL_STREAM_DRAW

class Model {
   public:
    Model();
    Model(GLenum usage);
    ~Model();
    void Bind() const;
    void Unbind() const;
    void SetVertexData(const GLvoid *data, const unsigned int count, const GLenum type);
    void SetIndexData(const GLuint *data, const unsigned int count);
    void SetBufferLayout(const std::vector<LayoutElement> layoutElements);
    void PackModel();
    void Draw() const;
    void SetPrimitive(const GLenum type, const GLfloat size);

   private:
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;
    VertexArray m_VAO;
    VertexBufferLayout m_BufferLayout;
    GLenum m_PrimitiveType;
    GLenum m_Usage;
};