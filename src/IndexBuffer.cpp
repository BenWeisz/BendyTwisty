#include "IndexBuffer.h"

IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &m_RendererID);
}

IndexBuffer::IndexBuffer(const GLvoid *data, const GLsizeiptr size) {
    glGenBuffers(1, &m_RendererID);
    Bind();
    SetData(data, size);
    Unbind();
}

IndexBuffer::~IndexBuffer() {
    IndexBuffer::Unbind();
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::SetData(const GLvoid *data, const GLsizeiptr size) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}