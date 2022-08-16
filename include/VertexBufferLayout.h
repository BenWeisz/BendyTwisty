#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <vector>

struct LayoutElement {
    GLint count;
    GLenum type;

    static unsigned int GetSizeOfType(const GLenum type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(GLfloat);
            default:
                return 4;
        }
    }
};

class VertexBufferLayout {
   private:
    std::vector<LayoutElement> m_LayoutElements;
    GLsizei m_Stride;

   public:
    VertexBufferLayout() : m_Stride(0) {}

    ~VertexBufferLayout() {}

    template <typename T>
    void Push(const GLint count) {
    }

    template <>
    void Push<GLfloat>(const GLint count) {
        LayoutElement element = {
            .count = count,
            .type = GL_FLOAT};

        m_LayoutElements.push_back(element);
        m_Stride += count * LayoutElement::GetSizeOfType(GL_FLOAT);
    }

    inline const std::vector<LayoutElement>& GetLayoutElements() const { return m_LayoutElements; }
    inline const GLsizei GetStride() const { return m_Stride; }
};