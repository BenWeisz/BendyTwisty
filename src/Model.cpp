#include "Model.h"

Model::Model() {}

Model::~Model() {}

void Model::Bind() const {
    m_VAO.Bind();
    m_IBO.Bind();
}

void Model::Unbind() const {
    m_IBO.Unbind();
    m_VAO.Unbind();
}

void Model::AddVertexData(const GLvoid *data, const unsigned int count, const GLenum type) {
    GLsizeiptr size;
    switch (type) {
        case GL_FLOAT:
            size = count * sizeof(GLfloat);
            break;
        default:
            size = count * 4;
            break;
    }

    m_VBOs.emplace_back(data, size);
}

void Model::SetIndexData(const GLuint *data, const unsigned int count) {
    m_IBO.Bind();
    m_IBO.SetData(data, count);
    m_IBO.Unbind();
}

void Model::AddBufferLayout(const std::vector<LayoutElement> layoutElements) {
    if (!layoutElements.size())
        return;

    m_BufferLayouts.emplace_back();

    for (auto &element : layoutElements) {
        GLint count = element.count;
        GLenum type = element.type;
        if (type == GL_FLOAT)
            m_BufferLayouts[m_BufferLayouts.size() - 1].Push<GLfloat>(count);
    }
}

void Model::PackModel() {
    assert(m_VBOs.size() == m_BufferLayouts.size());
    m_VAO.Bind();

    for (int i = 0; i < m_VBOs.size(); i++)
        m_VAO.AddBuffer(m_VBOs[i], m_BufferLayouts[i]);

    m_VAO.Unbind();
}

void Model::Draw() const {
    glDrawElements(GL_TRIANGLES, m_IBO.GetIndexCount(), GL_UNSIGNED_INT, (const GLvoid *)0);
}