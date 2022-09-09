#pragma once

#include <vector>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "Entity.h"
#include "VertexBufferLayout.h"
#include "Material.h"
#include "ShaderProgram.h"

class Rod : public Entity {
   public:
    Rod(const GLfloat length, const int segments) : Entity("Rod"), m_Length(length), m_Segments(segments) {
        InitModel();
        m_IsLightingEnabled = false;
        m_Metadata[ENTITY_SHOW_IN_GUI] = false;
    }

    void Update(const float deltaTime) override {
        m_Model->SetVertexData(NULL, 3 * (m_Segments + 1), GL_FLOAT);

        // Updates should be preformed here

        m_Model->SetVertexData(q.data(), 3 * (m_Segments + 1), GL_FLOAT);
    }

    void Draw(const float deltaTime) const override {
        m_Model->Bind();

        FlatMaterial* flatMaterial = (FlatMaterial*)GetActiveMaterial();
        ShaderProgram& shader = flatMaterial->shader;

        shader.SetUniform3fv("u_FlatColor", &(flatMaterial->color)[0]);

        m_Model->Draw();
        m_Model->Unbind();
    }

   private:
    void InitModel() {
        m_Model = new Model(MODEL_STREAMING);
        m_Model->SetPrimitive(GL_LINES, 1.0f);

        q = Eigen::MatrixXf(3, m_Segments + 1);

        float step = m_Length / m_Segments;
        float startX = -m_Length / 2.0f;
        for (int i = 0; i < m_Segments + 1; i++) {
            q(0, i) = startX + (i * step);
            q(1, i) = 0.0f;
            q(2, i) = 0.0f;
        }

        GLuint* indices = new GLuint[2 * m_Segments];

        for (int i = 1; i < m_Segments + 1; i++) {
            const int offset = (i - 1) * 2;
            indices[offset] = i - 1;
            indices[offset + 1] = i;
        }

        m_Model->SetVertexData(q.data(), 3 * (m_Segments + 1), GL_FLOAT);
        m_Model->SetIndexData(indices, 2 * m_Segments);

        std::vector<LayoutElement> layoutElements;
        layoutElements.push_back((LayoutElement){3, GL_FLOAT});

        m_Model->SetBufferLayout(layoutElements);
        m_Model->PackModel();

        delete[] indices;

        // Set up physics model
        qbar = q;
        qdot = Eigen::MatrixXf::Zero(3, m_Segments + 1);
        bFrame << 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, -1.0f, 0.0f;

        ebar = computeEdgeVectors(qbar);
        kb = computeCurvatureBinormal(ebar);

        std::cout << kb << std::endl;
    }

    Eigen::MatrixXf computeEdgeVectors(const Eigen::MatrixXf& x) {
        return x.block(0, 1, 3, m_Segments) - x.block(0, 0, 3, m_Segments);
    }

    Eigen::MatrixXf computeCurvatureBinormal(const Eigen::MatrixXf& e) {
        auto eim1 = e.block(0, 0, 3, m_Segments - 1);
        auto ei = e.block(0, 1, 3, m_Segments - 1);

        auto ebarmag = (ebar.transpose() * ebar).diagonal().cwiseSqrt();
        auto a = ebarmag.rows();
        auto b = ebarmag.cols();
        auto magprod = ebarmag.segment(0, m_Segments - 1).cwiseProduct(ebarmag.segment(1, m_Segments - 1));

        auto edot = (eim1.transpose() * ei).diagonal();

        auto bottom = magprod + edot;

        Eigen::MatrixXf binormals = Eigen::MatrixXf::Zero(3, m_Segments - 1);
        for (int i = 0; i < m_Segments - 1; i++) {
            Eigen::Vector3f e1 = eim1.col(i);
            Eigen::Vector3f e2 = ei.col(i);
            binormals.col(i) = e1.cross(e2) * 2 / bottom(i);
        }

        return binormals;
    }

   private:
    const GLfloat m_Length;
    const int m_Segments;
    Eigen::MatrixXf q;
    Eigen::MatrixXf qbar;
    Eigen::MatrixXf qdot;
    Eigen::Matrix3f bFrame;
    Eigen::MatrixXf ebar;
    Eigen::MatrixXf kb;
};