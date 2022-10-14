#pragma once

#include <Eigen/Core>

#include "../include/custom/rod/compute_strain.h"
#include "util.h"

TEST_CASE("Test edges equal", "[compute_strain]") {
    Eigen::MatrixXf e(3, 4);
    e << 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0;

    Eigen::VectorXf strain = compute_strain(e, e);
    REQUIRE(matrices_are_equal(strain, Eigen::VectorXf::Zero(4), 4));
}

TEST_CASE("Test edges are different", "[compute_strain]") {
    Eigen::MatrixXf e(3, 4);
    e << 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0;

    Eigen::MatrixXf ebar(3, 4);
    ebar << 0, 2, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0;

    Eigen::VectorXf strain = compute_strain(e, ebar);
    Eigen::VectorXf strain_correct(4);
    strain_correct << 0, -3, -12, -27;
    REQUIRE(matrices_are_equal(strain, strain_correct, 4));
}