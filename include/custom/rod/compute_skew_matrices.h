#pragma once

#include <vector>

#include <Eigen/Core>

// Compute the skew symmetric matrix for a
Eigen::Matrix3f compute_skew_matrix(const Eigen::Vector3f& a) {
    Eigen::Matrix3f skew_matrix;
    skew_matrix << 0, -a(2), a(1), a(2), 0, -a(0), -a(1), a(0), 0;
    return skew_matrix;
}

// Compute all the skew matrices for the vectors given as column vectors of A
std::vector<Eigen::Matrix3f> compute_skew_matrices(Eigen::MatrixXf& A) {
    std::vector<Eigen::Matrix3f> skew_matrices;
    for (int i = 0; i < A.cols(); i++) {
        skew_matrices.push_back(compute_skew_matrix(A.col(i)));
    }

    return skew_matrices;
}