#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <extern/glm/glm.hpp>
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
    void Draw() const;

   private:
    VertexArray *m_VAO;
    VertexBuffer *m_VBO;
    IndexBuffer *m_IBO;
    unsigned int m_IndexCount;
};