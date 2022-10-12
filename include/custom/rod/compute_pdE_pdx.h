#pragma once

#include <Eigen/Core>

#include <vector>

#include "compute_grad_omega.h"

// The partial derivative of the energy with respect to the vertex positions
typedef std::vector<Eigen::Vector3f> EPGradX;

EPGradX compute_pdE_pdx(
    Eigen::VectorXf& neighbor_len_bar, OmegaGrad& omega_grad, Eigen::Matrix2f& bending_modulus, Omega& omega, Omega& omega_bar) {
    int num_segments = omega.omega_j_i.cols() + 1;

    Eigen::MatrixXf B_omega_dif_j_im1 = bending_modulus * (omega.omega_j_im1 - omega_bar.omega_j_im1);
    Eigen::MatrixXf B_omega_dif_j_i = bending_modulus * (omega.omega_j_i - omega_bar.omega_j_i);

    EPGradX ep_grad_x;
    for (int i = 0; i < num_segments + 1; i++) {
        Eigen::Vector3f grad = Eigen::Vector3f::Zero();
        for (int k = 0; k < num_segments - 1; k++) {
            Eigen::Vector3f j_km1 = omega_grad[k].omega_grad_j_km1[i].transpose() * B_omega_dif_j_im1.col(k);
            Eigen::Vector3f j_k = omega_grad[k].omega_grad_j_k[i].transpose() * B_omega_dif_j_i.col(k);
            grad += (j_km1 + j_k) / neighbor_len_bar(k);
        }
        ep_grad_x.push_back(grad);
    }

    return ep_grad_x;
}