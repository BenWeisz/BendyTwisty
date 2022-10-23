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
#include "compute_hessian_d2Edtheta2.h"
#include "newtons_method_minimization.h"
#include "compute_grad_holonomy.h"
#include "compute_skew_matrices.h"
#include "compute_grad_kb.h"
#include "compute_grad_omega.h"
#include "compute_pdE_pdx.h"
#include "compute_pdE_pdtheta.h"
#include "compute_elastic_forces.h"
#include "time_integrate.h"
#include "compute_strain.h"
#include "compute_grad_strain.h"
#include "fast_manifold_projection.h"
#include "compute_updated_u0.h"

class Rod : public Entity {
   public:
    Rod(const GLfloat length, const int segments) : Entity("Rod"), m_Segments(segments) {
        InitModel(length);
        m_IsLightingEnabled = false;
        m_Metadata[ENTITY_SHOW_IN_GUI] = false;
    }

    void Update(const float deltaTime) override {
        m_Model->SetVertexData(NULL, 3 * (m_Segments + 1), GL_FLOAT);

        //
        // STEP 6
        //
        // Compute the forces on the centerline
        Eigen::MatrixXf e = compute_edges(m_x);
        std::vector<Eigen::Matrix3f> skew_matrices = compute_skew_matrices(e);

        Eigen::MatrixXf kb = compute_kb(e, m_ebar);
        KBGrad kb_grad = compute_grad_kb(skew_matrices, kb, e, m_ebar);

        PsiGrad psi_grad = compute_grad_holonomy(kb, m_ebar);
        PsiGradSum psi_grad_sum = compute_grad_holonomy_sum(psi_grad);

        // Compute the updated material frames
        std::vector<Eigen::Matrix3f> mf = compute_material_frame(m_bf, m_theta);

        // Compute the new omega and omega grad
        Omega omega = compute_omega(kb, mf);
        OmegaGrad omega_grad = compute_grad_omega(mf, kb_grad, omega, psi_grad_sum);

        Eigen::Matrix2f bending_modulus;
        bending_modulus << m_alpha, 0, 0, m_alpha;

        // Compute the sub-components of the negative force
        EPGradX pdEpdx = compute_pdE_pdx(m_NeighborLenBar, omega_grad, bending_modulus, omega, m_omega_bar);
        Eigen::VectorXf pdEpdtheta = compute_pdE_pdtheta(m_NeighborLenBar, omega, m_omega_bar, bending_modulus, m_beta, m_theta);

        FORCE force = compute_elastic_forces(pdEpdx, pdEpdtheta, psi_grad_sum, m_BoundryConditions);

        //
        // Step 7
        //
        // Integrate the centerline
        time_integrate(m_x, m_v, force, m_VertexMass, m_DeltaTime);

        //
        // Step 8
        //
        // Enforce inextensibility
        // [Goldenthal, 2006] paper mentions convergence occurs in about 3-5 iteration
        fast_manifold_projection(m_x, m_v, m_ebar, m_DeltaTime, m_VertexMass, 5);

        //
        // Step 10
        //
        // Update bishop frame
        // Update the initial bishop frame so the tangent is still tangent to the first edge
        m_u0 = compute_updated_u0(m_x, m_u0);

        // Update the bishop frames across the rod
        e = compute_edges(m_x);
        Eigen::VectorXf phi = compute_phi(e);
        m_bf = parallel_transport(m_u0, kb, phi);

        //
        // Step 11
        //
        // Update quasistatic frame
        kb = compute_kb(e, m_ebar);
        m_theta = newtons_method_minimization(
            m_NeighborLenBar,
            bending_modulus,
            m_omega_bar,
            m_beta,
            m_theta,
            m_BoundryConditions,
            m_bf,
            kb,
            m_NewtonsTol,
            m_NewtonsMaxIters);

        m_theta(m_Segments - 1) += m_DeltaTime;
        std::cout << m_x << "\n"
                  << std::endl;

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
    void InitModel(const float rodLength) {
        // ModelEngine Boiler Plate
        m_Model = new Model(MODEL_STREAMING);
        m_Model->SetPrimitive(GL_LINES, 1.0f);

        // Configure material parameters
        m_alpha = 1.0f;
        m_beta = 1.0f;
        m_VertexMass = 1.0f;
        m_NewtonsTol = 0.0001;
        m_NewtonsMaxIters = 10;
        m_Time = 0.0f;
        m_DeltaTime = 0.01;

        Eigen::Matrix2f bending_modulus;
        bending_modulus << m_alpha, 0, 0, m_alpha;

        // Set up the vertex positions
        m_x = Eigen::MatrixXf(3, m_Segments + 1);

        float step = rodLength / m_Segments;
        float startX = -rodLength / 2.0f;
        for (int i = 0; i < m_Segments + 1; i++) {
            m_x(0, i) = startX + (i * step);
            m_x(1, i) = 0.0f;
            m_x(2, i) = 0.0f;
        }

        // Save a copy of the original vertex points
        // m_x(1, 1) = 10;
        m_xbar = m_x;

        m_v = Eigen::MatrixXf::Zero(3, m_Segments + 1);
        m_vbar = m_v;

        // Set up the natural undeformed defining bishop frame with t, u, v being columnwise
        m_u0
            << 1,
            0, 0, 0, 0, 1, 0, -1, 0;

        // Set up the boundry conditions
        m_BoundryConditions = new char[m_Segments];
        for (int i = 0; i < m_Segments; i++)
            m_BoundryConditions[i] = VERTEX_STRESS_FREE;

        // Clamp the first and last vertices
        m_BoundryConditions[0] = VERTEX_CLAMPED;
        m_BoundryConditions[m_Segments - 1] = VERTEX_CLAMPED;

        // Compute the curvature binormal for parallel transport
        m_ebar = compute_edges(m_xbar);
        m_NeighborLenBar = compute_neighbor_len(m_ebar);

        Eigen::MatrixXf kbbar = compute_kb(m_ebar, m_ebar);

        // Compute the angles between consecutive edges
        Eigen::VectorXf phibar = compute_phi(m_ebar);

        // Parallel transport the u0 frame along the rod
        m_bf = parallel_transport(m_u0, kbbar, phibar);

        // Fix the thetas for the clamped frames
        m_theta = Eigen::VectorXf::Zero(m_Segments);
        m_theta(0) = 0.0f;                      // We clamp the first frame with zero twist
        m_theta(m_Segments - 1) = M_PI * 2.0f;  // We clamp the last boundary with a twist angle
        // The rest of the angles are updated in the quasistatic update

        // Compute the material frame for computing omega bar
        std::vector<Eigen::Matrix3f> mf = compute_material_frame(m_bf, m_theta);

        //
        // Step 1
        //
        // Compute the omega bar values
        m_omega_bar = compute_omega(kbbar, mf);

        //
        // Step 2
        //
        // Set quasistatic material frame

        m_theta = newtons_method_minimization(
            m_NeighborLenBar,
            bending_modulus,
            m_omega_bar,
            m_beta,
            m_theta,
            m_BoundryConditions,
            m_bf,
            kbbar,
            m_NewtonsTol,
            m_NewtonsMaxIters);

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
    const int m_Segments;
    Eigen::MatrixXf m_x;
    Eigen::MatrixXf m_xbar;
    Eigen::MatrixXf m_v;
    Eigen::MatrixXf m_vbar;

    Eigen::MatrixXf m_ebar;

    Eigen::Matrix3f m_u0;
    char* m_BoundryConditions;
    std::vector<Eigen::Matrix3f> m_bf;
    Eigen::VectorXf m_theta;
    Omega m_omega_bar;
    Eigen::VectorXf m_NeighborLenBar;

    float m_alpha;
    float m_beta;
    float m_Time;
    float m_DeltaTime;
    float m_NewtonsTol;
    int m_NewtonsMaxIters;
    float m_VertexMass;
};
