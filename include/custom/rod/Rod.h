#pragma once

#include <vector>
#include <cmath>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

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

        m_Model->SetVertexData(m_q.data(), 3 * (m_Segments + 1), GL_FLOAT);
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

        m_q = Eigen::MatrixXf(3, m_Segments + 1);

        float step = m_Length / m_Segments;
        float startX = -m_Length / 2.0f;
        for (int i = 0; i < m_Segments + 1; i++) {
            m_q(0, i) = startX + (i * step);
            m_q(1, i) = 0.0f;
            m_q(2, i) = 0.0f;
        }

        GLuint* indices = new GLuint[2 * m_Segments];

        for (int i = 1; i < m_Segments + 1; i++) {
            const int offset = (i - 1) * 2;
            indices[offset] = i - 1;
            indices[offset + 1] = i;
        }

        // Boiler Plate
        m_Model->SetVertexData(m_q.data(), 3 * (m_Segments + 1), GL_FLOAT);
        m_Model->SetIndexData(indices, 2 * m_Segments);
        std::vector<LayoutElement> layoutElements;
        layoutElements.push_back((LayoutElement){3, GL_FLOAT});
        m_Model->SetBufferLayout(layoutElements);
        m_Model->PackModel();
        delete[] indices;
    }

   private:
    const GLfloat m_Length;
    const int m_Segments;
    Eigen::MatrixXf m_q;
};