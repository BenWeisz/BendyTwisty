#pragma once

#include <vector>

#include <Eigen/Core>

#include "compute_grad_kb.h"
#include "compute_omega.h"
#include "compute_grad_holonomy.h"

typedef std::vector<Eigen::MatrixXf> OmegaGrad_I;
typedef std::vector<OmegaGrad_I> OmegaGrad;

// Tertiary Level
typedef struct OmegaGrad_J {
    Eigen::MatrixXf omega_j_km1;
    Eigen::MatrixXf omega_j_k;
} OmegaGrad_Js;

// Secondary Level
typedef std::vector<OmegaGrad_J> OmegaGrad_K;

// Primary Level
typedef std::vector<OmegaGrad_K> OmegaGrad;

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

    OmegaGrad omega_grad;
    for (int i = 0; i < num_segments + 1; i++) {
        OmegaGrad_K omega_grad_k;
        for (int k = 0; k < num_segments - 1; k++) {
            OmegaGrad_J omega_grad_j;
            Eigen::MatrixXf omega_grad_j_km1 = Eigen::MatrixXf::Zero(2, 3);
            Eigen::MatrixXf omega_grad_j_k = Eigen::MatrixXf::Zero(2, 3);

            if (k > 0) {
                omega_grad_j_km1 += -(J * (omega.omega_j_im1.col(k) * psi_grad_sum[i].col(k - 1).transpose()));
            }
            omega_grad_j_k += -(J * (omega.omega_j_i.col(k) * psi_grad_sum[i].col(k).transpose()));

            if (i == k - 1) {
                omega_grad_j_km1 += frame_stacks[k] * kb_grad.grad_kb_im1[k];
                omega_grad_j_k += frame_stacks[k + 1] * kb_grad.grad_kb_im1[k];
            } else if (i == k) {
                omega_grad_j_km1 += frame_stacks[k] * kb_grad.grad_kb_i[k];
                omega_grad_j_k += frame_stacks[k + 1] * kb_grad.grad_kb_i[k];
            } else if (i == k + 1) {
                omega_grad_j_km1 += frame_stacks[k] * kb_grad.grad_kb_ip1[k];
                omega_grad_j_k += frame_stacks[k + 1] * kb_grad.grad_kb_ip1[k];
            }

            omega_grad_j.omega_j_km1 = omega_grad_j_km1;
            omega_grad_j.omega_j_k = omega_grad_j_k;
            omega_grad_k.push_back(omega_grad_j);
        }
        omega_grad.push_back(omega_grad_k);
    }

    return omega_grad;
}

// OmegaGrad compute_grad_omega(std::vector<Eigen::Matrix3f>& mf, KBGrad& kb_grad, Omega& omega, PsiGradSum& psi_grad_sum) {
//     int num_segments = mf.size();

//     std::vector<Eigen::MatrixXf> frame_stacks;
//     for (int i = 0; i < num_segments; i++) {
//         Eigen::MatrixXf frame(2, 3);
//         frame.row(0) = mf[i].col(2);
//         frame.row(1) = -mf[i].col(1);
//     }

//     Eigen::Matrix2f J;
//     J << 0, -1, 1, 0;

//     OmegaGrad_I grad_j_km1;
//     OmegaGrad_I grad_j_k;

//     for (int k = 0; k < num_segments - 1; k++) {
//         Eigen::MatrixXf grad_j_km1_i_km1(2, 3);
//         grad_j_km1_i_km1 = frame_stacks[k] * kb_grad.grad_kb_im1[k];

//         Eigen::MatrixXf grad_j_km1_i_k(2, 3);
//         grad_j_km1_i_k = frame_stacks[k] * kb_grad.grad_kb_i[k];

//         Eigen::MatrixXf grad_j_km1_i_kp1(2, 3);
//         grad_j_km1_i_kp1 = frame_stacks[k] * kb_grad.grad_kb_ip1[k];

//         // if ()

//         grad_j_km1.grad_i_km1.push(grad_j_km1_i_km1);
//         grad_j_km1.grad_i_k.push_back(grad_j_km1_i_k);
//         grad_j_km1.grad_i_kp1.push_back(grad_j_km1_i_kp1);
//     }

//     OmegaGrad omega_grad;
//     omega_grad.grad_j_km1 = grad_j_km1;
//     omega_grad.grad_j_k = grad_j_k;

//     return omega_grad;
// }