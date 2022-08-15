#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &m_RendererID);
}

VertexBuffer::VertexBuffer(const GLvoid *data, const GLsizeiptr size) {
    glGenBuffers(1, &m_RendererID);
    Bind();
    SetData(data, size);
    Unbind();
}

void VertexBuffer::SetData(const GLvoid *data, const GLsizeiptr size) {
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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
