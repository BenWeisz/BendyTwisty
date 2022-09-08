#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &m_RendererID);
}

VertexBuffer::VertexBuffer(const GLvoid *data, const GLsizeiptr size, const GLenum usage) {
    glGenBuffers(1, &m_RendererID);
    Bind();
    SetData(data, size, usage);
    Unbind();
}

void VertexBuffer::SetData(const GLvoid *data, const GLsizeiptr size, const GLenum usage) {
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer() {
    VertexBuffer::Unbind();
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
