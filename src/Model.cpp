#include "Model.h"

Model::Model() {}

Model::~Model() {
    if (m_VBO != nullptr)
        delete m_VBO;

    if (m_IBO != nullptr)
        delete m_IBO;

    if (m_VAO != nullptr)
        delete m_VAO;
}

void Model::Bind() const {
    m_VAO->Bind();
    m_IBO->Bind();
}

void Model::Unbind() const {
    assert(m_VBO != nullptr);
    assert(m_IBO != nullptr);
    assert(m_VAO != nullptr);

    m_IBO->Unbind();
    m_VAO->Unbind();
}

void Model::SetVertexData(const GLfloat *data, const unsigned int count) {
    m_VBO = new VertexBuffer((const GLvoid *)data, count * sizeof(GLfloat));
}

void Model::SetIndexData(const GLuint *data, const unsigned int count) {
    m_IBO = new IndexBuffer((const GLvoid *)data, count * sizeof(GLuint));
}

void Model::PackModel(const std::vector<LayoutElement> &layoutElements) {
    if (m_VAO == nullptr) {
        assert(m_VBO != nullptr);
        assert(m_IBO != nullptr);

        VertexBufferLayout layout = VertexBufferLayout();
        layout.PushMany(layoutElements);

        m_VAO = new VertexArray();
        m_VAO->Bind();
        m_VAO->AddBuffer(*m_VBO, layout);
        m_VAO->Unbind();
    }
}