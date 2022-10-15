#pragma once

#include <Eigen/Core>
#include <Eigen/Sparse>

#include "../include/custom/rod/compute_grad_strain.h"
#include "util.h"

TEST_CASE("Test computing zero grad", "[compute_grad_strain]") {
    Eigen::MatrixXf e(3, 4);
    e << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

    Eigen::MatrixXf grad_strain = compute_grad_strain(e);
    REQUIRE(grad_strain.rows() == 4);
    REQUIRE(grad_strain.cols() == 3 * 5);
    REQUIRE(matrices_are_equal(grad_strain, Eigen::MatrixXf::Zero(4, 15), 60));
}

TEST_CASE("Test normal edges", "[compute_grad_strain]") {
    Eigen::MatrixXf e(3, 4);
    e << 1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12;

    Eigen::MatrixXf grad_strain = compute_grad_strain(e);
    REQUIRE(grad_strain.rows() == 4);
    REQUIRE(grad_strain.cols() == 3 * 5);

    Eigen::MatrixXf expected_grad_strain(4, 15);
    expected_grad_strain << -2, -10, -18, 2, 10, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, -4, -12, -20, 4, 12, 20, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, -6, -14, -22, 6, 14, 22, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, -8, -16, -24, 8, 16, 24;

    REQUIRE(matrices_are_equal(grad_strain, expected_grad_strain, 60));
}