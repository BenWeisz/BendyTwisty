#pragma once

#include <Eigen/Core>
#include "compute_omega.h"

Eigen::VectorXf compute_pdE_pdtheta(
    Eigen::VectorXf& neighbor_len_bar,
    Omega& omega,
    Omega& omega_bar,
    Eigen::Matrix2f& bending_modulus,
    float beta,
    Eigen::VectorXf& theta) {
    int num_segments = neighbor_len_bar.size() + 1;

    Eigen::Matrix2f J;
    J << 0, -1, 1, 0;

    Eigen::Matrix2f JB = J * bending_modulus;

    Eigen::MatrixXf omega_dif_ii = omega.omega_j_i - omega_bar.omega_j_i;
    Eigen::MatrixXf JB_omega_dif_ii = JB * omega_dif_ii;

    Eigen::VectorXf omega_term = (omega.omega_j_i.transpose() * JB_omega_dif_ii).diagonal();
    Eigen::VectorXf m = theta.segment(1, num_segments - 1) - theta.segment(0, num_segments - 1);

    Eigen::VectorXf beta_term = 2.0f * beta * (m.array() / neighbor_len_bar.array());

    Eigen::VectorXf pdE_pdtheta = omega_term + beta_term;
    return pdE_pdtheta;
}