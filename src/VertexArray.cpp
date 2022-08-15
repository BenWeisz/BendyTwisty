#include "VertexArray.h"

VertexArray::VertexArray() : m_NextAttr(0) {
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
    VertexArray::Unbind();
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) {
    vbo.Bind();

    const auto& elements = layout.GetLayoutElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(m_NextAttr + i);
        glVertexAttribPointer(m_NextAttr + i, element.count, element.type, GL_FALSE, layout.GetStride(), (const GLvoid*)offset);
        offset += element.count * LayoutElement::GetSizeOfType(element.type);
    }

    m_NextAttr += elements.size();

    vbo.Unbind();
}