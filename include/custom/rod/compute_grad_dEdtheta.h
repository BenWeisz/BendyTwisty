#pragma once

#include <vector>

#include <Eigen/Core>

#include "util.h"
#include "compute_omega.h"

// Compute the gradient of the elastic energy with respect to the twist angle theta
// This is used for enforcing a quasistatic material frame via newton's method
Eigen::VectorXf compute_grad_dEdtheta(
    Eigen::VectorXf& neighbor_len_bar,
    Omega& omega,
    Eigen::Matrix2f& bending_modulus,
    Omega& omega_bar,
    const float beta,
    Eigen::VectorXf& theta,
    char* boundry_conditions) {
    // Set up grad
    int num_segments = theta.size();
    Eigen::VectorXf grad = Eigen::VectorXf::Zero(num_segments);

    // J matrix rotates PI/2 CC
    Eigen::Matrix2f J;
    J << 0, -1, 1, 0;

    Eigen::Matrix2f JB = J * bending_modulus;

    // Compute the gradient of the W terms
    // dW_j/dtheta_j
    Eigen::VectorXf dW_j_dtheta_j;
    dW_j_dtheta_j = (omega.omega_j_i.transpose() * JB * (omega.omega_j_i - omega_bar.omega_j_i)).diagonal();
    dW_j_dtheta_j = dW_j_dtheta_j.array() / neighbor_len_bar.array();
    grad.segment(1, num_segments - 1) += dW_j_dtheta_j;

    // dW_j+1/dtheta_j
    Eigen::VectorXf dW_jp1_dtheta_j;
    dW_jp1_dtheta_j = (omega.omega_j_im1.transpose() * JB * (omega.omega_j_im1 - omega_bar.omega_j_im1)).diagonal();
    dW_jp1_dtheta_j = dW_jp1_dtheta_j.array() / neighbor_len_bar.array();
    grad.segment(0, num_segments - 1) += dW_jp1_dtheta_j;

    // Compute the twist difference m_i = theta_i - theta_i-1
    Eigen::VectorXf m = theta.segment(1, num_segments - 1) - theta.segment(0, num_segments - 1);
    Eigen::VectorXf ml = m.array() / neighbor_len_bar.array();

    grad.segment(1, num_segments - 1) += 2.0 * beta * ml;
    grad.segment(0, num_segments - 1) -= 2.0 * beta * ml;

    for (int i = 0; i < num_segments; i++)
        if (boundry_conditions[i] == EDGE_CLAMPED)
            grad(i) = 0.0f;

    return grad;
}