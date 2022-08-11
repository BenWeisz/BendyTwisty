#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <cassert>
#include <vector>

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
    void SetVertexData(const GLfloat *data, const unsigned int count);
    void SetIndexData(const GLuint *data, const unsigned int count);
    void PackModel(const std::vector<LayoutElement> &layoutElements);

   private:
    VertexArray *m_VAO;
    VertexBuffer *m_VBO;
    IndexBuffer *m_IBO;
};