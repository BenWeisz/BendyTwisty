#pragma once

#include <vector>

#include <Eigen/Core>

#define OMEGA_J_IM1 0
#define OMEGA_J_I 1

// Compute the values for omega_j^i
// If OMEGA_J_IM1 is specified then j=i-1 otherwise if OMEGA_J_I is specified then j=i
Eigen::MatrixXf compute_omega(Eigen::MatrixXf& kb, std::vector<Eigen::Matrix3f>& mf, const char type) {
    int num_segments = mf.size();

    assert(type == OMEGA_J_I || type == OMEGA_J_IM1);
    int start_index = type;  // for j=i-1, i=1 is valid for kb and so we can start from 0 for the material frames;

    Eigen::MatrixXf m1(3, num_segments - 1);
    Eigen::MatrixXf m2(3, num_segments - 1);
    for (int i = start_index; i < num_segments - 1 + start_index; i++) {
        int k = i - start_index;
        m1.col(k) = mf[i].col(1);
        m2.col(k) = mf[i].col(2);
    }

    Eigen::MatrixXf omega(2, num_segments - 1);
    omega.row(0) = (kb.transpose() * m2).diagonal();
    omega.row(1) = (-kb.transpose() * m1).diagonal();

    return omega;
}