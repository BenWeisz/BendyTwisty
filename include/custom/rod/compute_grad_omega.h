#pragma once

#include <vector>

#include <Eigen/Core>

#include "compute_grad_kb.h"
#include "compute_omega.h"
#include "compute_grad_holonomy.h"

typedef struct OmegaGrad_I {
    std::vector<Eigen::MatrixXf> grad_i_km1;
    std::vector<Eigen::MatrixXf> grad_i_k;
    std::vector<Eigen::MatrixXf> grad_i_kp1;
} OmegaGrad_I;

typedef struct OmegaGrad {
    OmegaGrad_I grad_j_km1;
    OmegaGrad_I grad_j_k;
} OmegaGrad;

// Compute the gradient of omega
OmegaGrad compute_grad_omega(std::vector<Eigen::Matrix3f>& mf, KBGrad& kb_grad, Omega& omega, PsiGradSum& psi_grad_sum) {
    int num_segments = mf.size();

    std::vector<Eigen::MatrixXf> frame_stacks;
    for (int i = 0; i < num_segments; i++) {
        Eigen::MatrixXf frame(2, 3);
        frame.row(0) = mf[i].col(2);
        frame.row(1) = -mf[i].col(1);
    }

    Eigen::Matrix2f J;
    J << 0, -1, 1, 0;

    OmegaGrad_I grad_j_km1;
    OmegaGrad_I grad_j_k;

    for (int k = 0; k < num_segments - 1; k++) {
        Eigen::MatrixXf grad_j_km1_i_km1(2, 3);
        grad_j_km1_i_km1 = frame_stacks[k] * kb_grad.grad_kb_im1[k];

        Eigen::MatrixXf grad_j_km1_i_k(2, 3);
        grad_j_km1_i_k = frame_stacks[k] * kb_grad.grad_kb_i[k];

        Eigen::MatrixXf grad_j_km1_i_kp1(2, 3);
        grad_j_km1_i_kp1 = frame_stacks[k] * kb_grad.grad_kb_ip1[k];

        // if ()

        grad_j_km1.grad_i_km1.push(grad_j_km1_i_km1);
        grad_j_km1.grad_i_k.push_back(grad_j_km1_i_k);
        grad_j_km1.grad_i_kp1.push_back(grad_j_km1_i_kp1);
    }

    OmegaGrad omega_grad;
    omega_grad.grad_j_km1 = grad_j_km1;
    omega_grad.grad_j_k = grad_j_k;

    return omega_grad;
}