#pragma once

#include <vector>
#include <Eigen/Core>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <limits>

#include "../include/custom/rod/parallel_transport.h"

TEST_CASE("Rotate from axis to axis", "[rotate_about_unit_axis]") {
    Eigen::Vector3f v;
    v << 1, 0, 0;

    Eigen::Vector3f k;
    k << 0, 0, 1;

    Eigen::Vector3f vk = rotate_about_unit_axis(v, k, M_PI / 2);
    REQUIRE(std::abs(vk(0)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
    REQUIRE(vk(1) == Catch::Approx(1));
    REQUIRE(std::abs(vk(2)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
}

TEST_CASE("Rotate from plane to plane", "[rotate_about_unit_axis]") {
    Eigen::Vector3f v;
    v << 1, 1, 0;

    Eigen::Vector3f k;
    k << 1, 0, 0;

    Eigen::Vector3f vk = rotate_about_unit_axis(v, k, M_PI / 2);
    REQUIRE(vk(0) == Catch::Approx(1.0f));
    REQUIRE(std::abs(vk(1)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
    REQUIRE(vk(2) == Catch::Approx(1.0f));
}

TEST_CASE("Rotation preserves magnitude", "[rotate_about_unit_axis]") {
    Eigen::Vector3f v;
    v << 100, 0, 0;

    Eigen::Vector3f k;
    k << 0, 0, 1;

    Eigen::Vector3f vk = rotate_about_unit_axis(v, k, M_PI / 2);
    REQUIRE(std::abs(vk(0)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
    REQUIRE(vk(1) == Catch::Approx(100));
    REQUIRE(std::abs(vk(2)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
}

// TEST_CASE("Straight Rod", "[parallel_transport]") {
//     Eigen::Matrix3f u0;
//     u0 << 1, 0, 0, 0, 0, 1, 0, -1, 0;
// }