#pragma once

#include <vector>

#include <Eigen/Core>

#include "compute_grad_kb.h"
#include "compute_omega.h"
#include "compute_grad_holonomy.h"

typedef std::vector<Eigen::MatrixXf> OmegaGrad_K_J;
typedef struct OmegaGrad_K {
    OmegaGrad_K_J omega_grad_j_km1;
    OmegaGrad_K_J omega_grad_j_k;
} OmegaGrad_K;

typedef std::vector<OmegaGrad_K> OmegaGrad;

// Compute the gradient of omega
OmegaGrad compute_grad_omega(std::vector<Eigen::Matrix3f>& mf, KBGrad& kb_grad, Omega& omega, PsiGradSum& psi_grad_sum) {
    int num_segments = mf.size();

    std::vector<Eigen::MatrixXf> frame_stacks;
    for (int i = 0; i < num_segments; i++) {
        Eigen::MatrixXf frame(2, 3);
        frame.row(0) = mf[i].col(2);
        frame.row(1) = -mf[i].col(1);
        frame_stacks.push_back(frame);
    }

    Eigen::Matrix2f J;
    J << 0, -1, 1, 0;

    OmegaGrad omega_grad;
    for (int k = 0; k < num_segments - 1; k++) {
        OmegaGrad_K omega_grad_k;

        // Repeat this twice to build omega_grad_k
        OmegaGrad_K_J omega_grad_j_km1;
        for (int i = 0; i < num_segments + 1; i++) {
            Eigen::MatrixXf omega_grad_k_j_i = Eigen::MatrixXf::Zero(2, 3);
            Eigen::MatrixXf frame = frame_stacks[k];
            if (i == k) {
                omega_grad_k_j_i += frame * kb_grad.grad_kb_im1[k];
            } else if (i == k + 1) {
                omega_grad_k_j_i += frame * kb_grad.grad_kb_i[k];
            } else if (i == k + 2) {
                omega_grad_k_j_i += frame * kb_grad.grad_kb_ip1[k];
            }

            if (k > 0) {
                int j = k - 1;
                omega_grad_k_j_i -= J * (omega.omega_j_im1.col(k) * psi_grad_sum[i].col(j).transpose());
            }

            omega_grad_j_km1.push_back(omega_grad_k_j_i);
        }

        // Do the same loop here
        OmegaGrad_K_J omega_grad_j_k;
        for (int i = 0; i < num_segments + 1; i++) {
            Eigen::MatrixXf omega_grad_k_j_i = Eigen::MatrixXf::Zero(2, 3);
            Eigen::MatrixXf frame = frame_stacks[k + 1];
            if (i == k) {
                omega_grad_k_j_i += frame * kb_grad.grad_kb_im1[k];
            } else if (i == k + 1) {
                omega_grad_k_j_i += frame * kb_grad.grad_kb_i[k];
            } else if (i == k + 2) {
                omega_grad_k_j_i += frame * kb_grad.grad_kb_ip1[k];
            }

            omega_grad_k_j_i -= J * (omega.omega_j_i.col(k) * psi_grad_sum[i].col(k).transpose());
            omega_grad_j_k.push_back(omega_grad_k_j_i);
        }

        // Populate omega_grad_k
        omega_grad_k.omega_grad_j_km1 = omega_grad_j_km1;
        omega_grad_k.omega_grad_j_k = omega_grad_j_k;

        omega_grad.push_back(omega_grad_k);
    }

    return omega_grad;
}