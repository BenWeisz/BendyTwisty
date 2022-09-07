#include "Model.h"

Model::Model() : m_PrimitiveType(GL_TRIANGLES) {}

Model::~Model() {}

void Model::Bind() const {
    m_VAO.Bind();
    m_IBO.Bind();
}

void Model::Unbind() const {
    m_IBO.Unbind();
    m_VAO.Unbind();
}

void Model::SetVertexData(const GLvoid *data, const unsigned int count, const GLenum type) {
    GLsizeiptr size;
    switch (type) {
        case GL_FLOAT:
            size = count * sizeof(GLfloat);
            break;
        default:
            size = count * 4;
            break;
    }

    m_VBO.Bind();
    m_VBO.SetData(data, size);
    m_VBO.Unbind();
}

void Model::SetIndexData(const GLuint *data, const unsigned int count) {
    m_IBO.Bind();
    m_IBO.SetData(data, count);
    m_IBO.Unbind();
}

void Model::SetBufferLayout(const std::vector<LayoutElement> layoutElements) {
    if (!layoutElements.size())
        return;

    for (auto &element : layoutElements) {
        GLint count = element.count;
        GLenum type = element.type;
        if (type == GL_FLOAT)
            m_BufferLayout.Push<GLfloat>(count);
    }
}

void Model::PackModel() {
    m_VAO.Bind();
    m_VAO.SetBuffer(m_VBO, m_BufferLayout);
    m_VAO.Unbind();
}

void Model::Draw() const {
    glDrawElements(m_PrimitiveType, m_IBO.GetIndexCount(), GL_UNSIGNED_INT, (const GLvoid *)0);
}

void Model::SetPrimitive(const GLenum type, const GLfloat size) {
    assert(type == GL_TRIANGLES || type == GL_LINES || type == GL_POINTS);
    m_PrimitiveType = type;

    if (type == GL_LINES)
        glLineWidth(size);
    else if (type == GL_POINTS)
        glPointSize(size);
}
