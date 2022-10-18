#pragma once

#include <Eigen/Core>
#include <iostream>

#include "../include/custom/rod/compute_updated_u0.h"
#include "util.h"

TEST_CASE("Test bishop frame rotation", "[compute_updated_u0]") {
    Eigen::MatrixXf x(3, 2);
    x << 0.0, 0.9f, 0.0f, 0.5f, 0.0f, 0.8f;

    Eigen::Matrix3f u0;
    u0 << 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f;

    Eigen::Matrix3f u0_new = compute_updated_u0(x, u0);
    Eigen::Matrix3f expected;
    expected << 0.69026849, 0.61357199, -0.38348249, 0.38348249, 0.13920517, 0.91299677, 0.61357199, -0.77727172, -0.13920517;

    std::cout << u0_new << std::endl;

    REQUIRE(matrices_are_equal(u0_new, expected, 9));
}