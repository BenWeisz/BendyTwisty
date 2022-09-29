#pragma once

#include <vector>

#include <Eigen/Core>

typedef struct Omega {
    Eigen::MatrixXf omega_j_im1;
    Eigen::MatrixXf omega_j_i;
} Omega;

// Compute the values for omega_j^im1 and omega_j^i
Omega compute_omega(Eigen::MatrixXf& kb, std::vector<Eigen::Matrix3f>& mf) {
    int num_segments = mf.size();

    Omega omega;
    Eigen::MatrixXf m1(3, num_segments);
    Eigen::MatrixXf m2(3, num_segments);
    for (int i = 0; i < num_segments; i++) {
        m1.col(i) = mf[i].col(1);
        m2.col(i) = mf[i].col(2);
    }

    omega.omega_j_im1.resize(2, num_segments - 1);
    omega.omega_j_im1.row(0) = (kb.transpose() * m2.block(0, 0, 3, num_segments - 1)).diagonal();
    omega.omega_j_im1.row(1) = (-kb.transpose() * m1.block(0, 0, 3, num_segments - 1)).diagonal();

    omega.omega_j_i.resize(2, num_segments - 1);
    omega.omega_j_i.row(0) = (kb.transpose() * m2.block(0, 1, 3, num_segments - 1)).diagonal();
    omega.omega_j_i.row(1) = (-kb.transpose() * m1.block(0, 1, 3, num_segments - 1)).diagonal();

    return omega;
}