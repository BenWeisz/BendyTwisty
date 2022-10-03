#pragma once

#include <Eigen/Core>
#include <cmath>
#include <catch2/catch_approx.hpp>

#include "../include/custom/rod/compute_phi.h"

TEST_CASE("Square Loop", "[compute_phi]") {
    const int num_vertices = 5;

    Eigen::MatrixXf e = Eigen::MatrixXf::Zero(3, num_vertices - 1);
    e.col(0) = Eigen::Vector3f({2, 0, 0});
    e.col(1) = Eigen::Vector3f({0, -2, 0});
    e.col(2) = Eigen::Vector3f({-2, 0, 0});
    e.col(3) = Eigen::Vector3f({0, 2, 0});

    Eigen::VectorXf phi = compute_phi(e);
    REQUIRE(phi(0) == Catch::Approx(M_PI / 2.0));
    REQUIRE(phi(1) == Catch::Approx(M_PI / 2.0));
    REQUIRE(phi(2) == Catch::Approx(M_PI / 2.0));
}

TEST_CASE("Curved Rod", "[compute_phi]") {
    const int num_vertices = 5;

    Eigen::MatrixXf e = Eigen::MatrixXf::Zero(3, num_vertices - 1);
    e.col(0) = Eigen::Vector3f({1, 0, 0});
    e.col(1) = Eigen::Vector3f({1, -1, 0});
    e.col(2) = Eigen::Vector3f({0, -1, 0});
    e.col(3) = Eigen::Vector3f({-1, -1, 0});

    Eigen::VectorXf phi = compute_phi(e);
    REQUIRE(phi(0) == Catch::Approx(M_PI / 4.0));
    REQUIRE(phi(1) == Catch::Approx(M_PI / 4.0));
    REQUIRE(phi(2) == Catch::Approx(M_PI / 4.0));
}