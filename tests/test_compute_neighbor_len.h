#pragma once

#include <Eigen/Core>

#include "../include/custom/rod/compute_neighbor_len.h"

TEST_CASE("Straight rod neighborhood lengths", "[compute_neighbor_len]") {
    Eigen::MatrixXf e(3, 4);
    e << 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0;

    Eigen::VectorXf l = compute_neighbor_len(e);

    REQUIRE(l.size() == 3);

    REQUIRE(l(0) == 2);
    REQUIRE(l(1) == 2);
    REQUIRE(l(2) == 2);
}

TEST_CASE("Variable length edges", "[compute_neighbor_len]") {
    Eigen::MatrixXf e(3, 4);
    e << 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0;

    Eigen::VectorXf l = compute_neighbor_len(e);

    REQUIRE(l.size() == 3);

    REQUIRE(l(0) == 3);
    REQUIRE(l(1) == 5);
    REQUIRE(l(2) == 7);
}