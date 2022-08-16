#include "IndexBuffer.h"

IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &m_RendererID);
}

IndexBuffer::IndexBuffer(const GLvoid *data, const GLsizei count) {
    glGenBuffers(1, &m_RendererID);
    Bind();
    SetData(data, count);
    Unbind();
}

IndexBuffer::~IndexBuffer() {
    IndexBuffer::Unbind();
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::SetData(const GLvoid *data, const GLsizei count) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
    m_IndexCount = count;
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLsizei IndexBuffer::GetIndexCount() const {
    return m_IndexCount;
}