#pragma once

#include <Eigen/Core>

// Triadiagonal solver
// This algorithm was found on wikipedia and verified to work
// https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm

Eigen::VectorXf tridiagonal_solve(Eigen::VectorXf &a, Eigen::VectorXf &b, Eigen::VectorXf &c, Eigen::VectorXf &d) {
    int N = a.size();
    for (int i = 1; i < N; i++) {
        float w = a(i) / b(i - 1);
        b(i) -= w * c(i - 1);
        d(i) -= w * d(i - 1);
    }

    Eigen::VectorXf x = Eigen::VectorXf::Zero(N);
    x(N - 1) = d(N - 1) / b(N - 1);
    for (int i = N - 2; i >= 0; i--)
        x(i) = (d(i) - (c(i) * x(i + 1))) / b(i);

    return x;
}