#pragma once

#include <Eigen/Core>
#include <catch2/catch_approx.hpp>

#include "../include/math/tridiagonal_solve.h"
#include "util.h"

TEST_CASE("Tridiagonal Solve", "[tridiagonal_solve]") {
    int N = 10;

    Eigen::VectorXf lower = Eigen::VectorXf::Random(N);
    Eigen::VectorXf center = Eigen::VectorXf::Random(N);
    Eigen::VectorXf upper = Eigen::VectorXf::Random(N);

    Eigen::VectorXf b = Eigen::VectorXf::Ones(N);

    Eigen::MatrixXf A = Eigen::MatrixXf::Zero(N, N);
    // Set up A Matrix
    for (int i = 0; i < N; i++)
        A(i, i) = center(i);

    for (int i = 1; i < N; i++)
        A(i, i - 1) = lower(i);

    for (int i = 0; i < N - 1; i++)
        A(i, i + 1) = upper(i);

    Eigen::VectorXf x = tridiagonal_solve(lower, center, upper, b);

    Eigen::VectorXf test_b = A * x;
    REQUIRE(matrices_are_equal(test_b, Eigen::VectorXf::Ones(N), N));
}