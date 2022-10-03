#pragma once

#include <Eigen/Core>

#include "../include/custom/rod/compute_edges.h"

TEST_CASE("Compute straight edges", "[compute_edges]") {
    const int num_vertices = 5;

    Eigen::MatrixXf x = Eigen::MatrixXf::Zero(3, num_vertices);
    x(0, 1) = 1;
    x(0, 2) = 2;
    x(0, 3) = 3;
    x(0, 4) = 4;

    Eigen::MatrixXf e = compute_edges(x);

    REQUIRE(e.rows() == 3);
    REQUIRE(e.cols() == num_vertices - 1);

    REQUIRE(e(0, 0) == 1);
    REQUIRE(e(0, 1) == 1);
    REQUIRE(e(0, 2) == 1);
    REQUIRE(e(0, 3) == 1);
}

TEST_CASE("Compute angled edges", "[compute_edges]") {
    const int num_vertices = 5;
    Eigen::MatrixXf x(3, num_vertices);

    x << 1, 2, 3, 4, 5,
        1, 2, 3, 4, 5,
        1, 2, 3, 4, 5;

    Eigen::MatrixXf e = compute_edges(x);
    Eigen::Vector3f ones = Eigen::Vector3f::Ones();

    REQUIRE(e.col(0).dot(ones) == 3);
    REQUIRE(e.col(1).dot(ones) == 3);
    REQUIRE(e.col(2).dot(ones) == 3);
    REQUIRE(e.col(3).dot(ones) == 3);
}

TEST_CASE("Increasing distances", "[compute_edges]") {
    const int num_vertices = 5;
    Eigen::MatrixXf x(3, num_vertices);

    x << 1, 2, 4, 8, 16,
        1, 2, 4, 8, 16,
        1, 2, 4, 8, 16;

    Eigen::MatrixXf e = compute_edges(x);
    Eigen::Vector3f ones = Eigen::Vector3f::Ones();

    REQUIRE(e.col(0).dot(ones) == 3);
    REQUIRE(e.col(1).dot(ones) == 6);
    REQUIRE(e.col(2).dot(ones) == 12);
    REQUIRE(e.col(3).dot(ones) == 24);
}