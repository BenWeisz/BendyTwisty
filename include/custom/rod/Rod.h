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

#include "util.h"
#include "compute_edges.h"
#include "compute_kb.h"
#include "compute_phi.h"
#include "parallel_transport.h"
#include "compute_material_frame.h"
#include "compute_omega.h"
#include "compute_neighbor_len.h"
#include "compute_grad_dEdtheta.h"

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

        m_Model->SetVertexData(m_x.data(), 3 * (m_Segments + 1), GL_FLOAT);
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

        // Configure material parameters
        m_alpha = 1;
        m_beta = 1;

        // Set up the vertex positions
        m_x = Eigen::MatrixXf(3, m_Segments + 1);

        float step = m_Length / m_Segments;
        float startX = -m_Length / 2.0f;
        for (int i = 0; i < m_Segments + 1; i++) {
            m_x(0, i) = startX + (i * step);
            m_x(1, i) = 0.0f;
            m_x(2, i) = 0.0f;
        }

        // Save a copy of the original vertex points
        // m_x(1, 1) = 10;
        m_xbar = m_x;

        // Set up the natural undeformed defining bishop frame with t, u, v being columnwise
        m_u0 << 1, 0, 0, 0, 0, 1, 0, -1, 0;

        // Set up the boundry conditions
        m_BoundryConditions = new char[m_Segments + 1];
        for (int i = 0; i < m_Segments + 1; i++)
            m_BoundryConditions[i] = VERTEX_STRESS_FREE;

        // Clamp the first and last vertices
        m_BoundryConditions[0] = VERTEX_CLAMPED;
        m_BoundryConditions[m_Segments] = VERTEX_CLAMPED;

        // Compute the curvature binormal for parallel transport
        Eigen::MatrixXf ebar = compute_edges(m_xbar);
        Eigen::MatrixXf kbbar = compute_kb(ebar, ebar);

        // Compute the angles between consecutive edges
        Eigen::VectorXf phibar = compute_phi(ebar);

        // Parallel transport the u0 frame along the rod
        m_bf = parallel_transport(m_u0, kbbar, phibar);

        // Initialize the twist angles for each of the material frames
        m_theta = Eigen::VectorXf::Zero(m_Segments);
        for (int i = 0; i < m_Segments; i++) {
            m_theta(i) = i * (3.141592 / m_Segments);
        }

        // Compute the material frame for computing omega bar
        std::vector<Eigen::Matrix3f> mf = compute_material_frame(m_bf, m_theta);

        // Compute the omega bar values
        m_omega_bar_j_im1 = compute_omega(kbbar, mf, OMEGA_J_IM1);
        m_omega_bar_j_i = compute_omega(kbbar, mf, OMEGA_J_I);

        Eigen::VectorXf neighbor_len_bar = compute_neighbor_len(ebar);

        Eigen::Matrix2f bending_modulus;
        bending_modulus << m_alpha, 0, 0, m_alpha;

        Eigen::VectorXf grad = compute_grad_dEdtheta(
            neighbor_len_bar,
            m_omega_bar_j_im1,
            m_omega_bar_j_i,
            bending_modulus,
            m_omega_bar_j_im1,
            m_omega_bar_j_i,
            m_beta,
            m_theta,
            m_BoundryConditions);

        std::cout << grad << std::endl;

        // Boiler Plate
        // Set up the correct indicies for the vertex data
        GLuint* indices = new GLuint[2 * m_Segments];

        for (int i = 1; i < m_Segments + 1; i++) {
            const int offset = (i - 1) * 2;
            indices[offset] = i - 1;
            indices[offset + 1] = i;
        }

        m_Model->SetVertexData(m_x.data(), 3 * (m_Segments + 1), GL_FLOAT);
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
    Eigen::MatrixXf m_x;
    Eigen::MatrixXf m_xbar;
    Eigen::Matrix3f m_u0;
    char* m_BoundryConditions;
    std::vector<Eigen::Matrix3f> m_bf;
    Eigen::VectorXf m_theta;
    Eigen::MatrixXf m_omega_bar_j_im1;
    Eigen::MatrixXf m_omega_bar_j_i;
    float m_alpha;
    float m_beta;
};