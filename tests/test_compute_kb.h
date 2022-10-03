#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cmath>

#include "../include/custom/rod/compute_kb.h"

TEST_CASE("Straight Rod kb", "[compute_kb]") {
    const int num_vertices = 5;

    Eigen::MatrixXf e = Eigen::MatrixXf::Zero(3, num_vertices - 1);
    e(0, 0) = 1;
    e(0, 1) = 1;
    e(0, 2) = 1;
    e(0, 3) = 1;

    Eigen::MatrixXf kb = compute_kb(e, e);
    Eigen::Vector3f ones = Eigen::Vector3f::Ones();

    // Proper return size
    REQUIRE(kb.cols() == num_vertices - 2);
    REQUIRE(kb.rows() == 3);

    // The kb vectors should be zero
    REQUIRE(kb.col(0).dot(ones) == 0);
    REQUIRE(kb.col(1).dot(ones) == 0);
    REQUIRE(kb.col(2).dot(ones) == 0);
}

TEST_CASE("Square loop", "[compute_kb]") {
    const int num_vertices = 5;

    Eigen::MatrixXf e = Eigen::MatrixXf::Zero(3, num_vertices - 1);
    e.col(0) = Eigen::Vector3f({2, 0, 0});
    e.col(1) = Eigen::Vector3f({0, -2, 0});
    e.col(2) = Eigen::Vector3f({-2, 0, 0});
    e.col(3) = Eigen::Vector3f({0, 2, 0});

    Eigen::MatrixXf kb = compute_kb(e, e);
    Eigen::Vector3f ones = Eigen::Vector3f::Ones();

    // All of the kb values should be (0, 0, -2)
    REQUIRE(kb.col(0).dot(ones) == -2);
    REQUIRE(kb.col(1).dot(ones) == -2);
    REQUIRE(kb.col(2).dot(ones) == -2);

    Eigen::VectorXf kb_mag = (kb.transpose() * kb).diagonal().cwiseSqrt();
    float k_mag = 2 * tan((M_PI / 2.0) / 2.0);

    // Test the kb vector norms
    REQUIRE(kb_mag(0) == k_mag);
    REQUIRE(kb_mag(1) == k_mag);
    REQUIRE(kb_mag(2) == k_mag);
}