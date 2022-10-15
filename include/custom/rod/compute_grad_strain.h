#pragma once

#include <vector>

#include <Eigen/Core>
#include <Eigen/Sparse>

// The strain depends on both e and ebar, but the gradient only depends on e
Eigen::SparseMatrix<float> compute_grad_strain(Eigen::MatrixXf& e) {
    int num_segments = e.cols();

    Eigen::SparseMatrix<float> grad_strain(num_segments, 3 * (num_segments + 1));
    grad_strain.setZero();

    std::vector<Eigen::Triplet<float>> coeffs;
    for (int i = 0; i < num_segments; i++) {
        int offset = i * 3;
        Eigen::VectorXf edge = e.col(i);
        coeffs.push_back(Eigen::Triplet<float>(i, offset + 0, -2.0f * edge(0)));
        coeffs.push_back(Eigen::Triplet<float>(i, offset + 1, -2.0f * edge(1)));
        coeffs.push_back(Eigen::Triplet<float>(i, offset + 2, -2.0f * edge(2)));
        coeffs.push_back(Eigen::Triplet<float>(i, offset + 3, 2.0f * edge(0)));
        coeffs.push_back(Eigen::Triplet<float>(i, offset + 4, 2.0f * edge(1)));
        coeffs.push_back(Eigen::Triplet<float>(i, offset + 5, 2.0f * edge(2)));
    }

    grad_strain.setFromTriplets(coeffs.begin(), coeffs.end());

    return grad_strain;
}