#pragma once

#include <vector>

#include <Eigen/Core>

#include "compute_pdE_pdx.h"
#include "compute_grad_holonomy.h"
#include "util.h"

typedef Eigen::VectorXf FORCE;

FORCE compute_elastic_forces(EPGradX& pdE_pdx, Eigen::VectorXf& pdE_pdtheta, PsiGradSum& psi_grad_sum, char* boundary_conditions) {
    int num_segments = pdE_pdtheta.size();

    FORCE elastic_force = Eigen::VectorXf::Zero((num_segments + 1) * 3);

    bool is_clamped = false;
    for (int i = 0; i < num_segments; i++) {
        if (boundary_conditions[i] == VERTEX_CLAMPED) {
            is_clamped = true;
            break;
        }
    }

    // Note: We might need to clamp the force in case the curvature gets really high
    for (int i = 0; i < num_segments + 1; i++) {
        elastic_force.segment(i * 3, 3) = -pdE_pdx[i];
        if (is_clamped) {
            float a = pdE_pdtheta(num_segments - 1);
            elastic_force.segment(i * 3, 3) += pdE_pdtheta(num_segments - 1) * psi_grad_sum[i].col(num_segments - 2);
        }
    }

    return elastic_force;
}