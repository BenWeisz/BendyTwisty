#pragma once

#include <Eigen/Core>
#include <catch2/catch_approx.hpp>

#include "../include/custom/rod/compute_first_u0.h"

TEST_CASE("Test orthogonality of frame", "[test_compute_first_u0]") {
    // Create the generating edge
    Eigen::Vector3f e;
    e << 1, 1, 0;

    Eigen::Matrix3f u0 = compute_first_u0(e);

    REQUIRE(u0.col(0).dot(u0.col(1)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
    REQUIRE(u0.col(0).dot(u0.col(2)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
    REQUIRE(u0.col(1).dot(u0.col(2)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
}