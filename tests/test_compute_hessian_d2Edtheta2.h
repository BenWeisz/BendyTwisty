#pragma once

#include <cmath>
#include <iostream>

#include <Eigen/Core>
#include <catch2/catch_approx.hpp>

#include "../include/custom/rod/compute_omega.h"
#include "../include/custom/rod/compute_hessian_d2Edtheta2.h"
#include "../include/custom/rod/util.h"
#include "util.h"

TEST_CASE("Square loop twisted rod", "[compute_hessian_d2Edtheta2]") {
    Eigen::VectorXf neighbor_len_bar(3);
    neighbor_len_bar << 2, 2, 2;

    float s = sin(M_PI / 4.0f);
    float s2 = s * 2.0f;

    Eigen::MatrixXf omega_j_im1(2, 3);
    omega_j_im1 << 0, -s2, -2, -2, -s2, 0;

    Eigen::MatrixXf omega_j_i(2, 3);
    omega_j_i << -s2, -2, -s2, -s2, 0, s2;

    Omega omega;
    omega.omega_j_im1 = omega_j_im1;
    omega.omega_j_i = omega_j_i;

    Eigen::MatrixXf omega_j_im1_bar(2, 3);
    omega_j_im1_bar << 0, 0, 0, -2, -2, -2;

    Eigen::MatrixXf omega_j_i_bar(2, 3);
    omega_j_i_bar << 0, 0, 0, -2, -2, -2;

    Omega omega_bar;
    omega_bar.omega_j_im1 = omega_j_im1_bar;
    omega_bar.omega_j_i = omega_j_i_bar;

    Eigen::Matrix2f bending_modulus;
    bending_modulus << 1, 0, 0, 1;

    float beta = 1;

    Eigen::VectorXf theta(4);
    theta << 0, M_PI / 4.0f, M_PI / 2.0f, (3.0f * M_PI) / 4.0f;

    SECTION("Free ends") {
        char boundary_conditions[4];
        boundary_conditions[0] = EDGE_STRESS_FREE;
        boundary_conditions[1] = EDGE_STRESS_FREE;
        boundary_conditions[2] = EDGE_STRESS_FREE;
        boundary_conditions[3] = EDGE_STRESS_FREE;

        Hessian hessian = compute_hessian_d2Edtheta2(
            beta,
            neighbor_len_bar,
            omega,
            bending_modulus,
            omega_bar,
            boundary_conditions);

        REQUIRE(hessian.lower.size() == 4);
        REQUIRE(hessian.center.size() == 4);
        REQUIRE(hessian.upper.size() == 4);

        Eigen::VectorXf lower(4);
        lower << 0.0f, -1.0f, -1.0f, -1.0f;

        Eigen::VectorXf center(4);
        center << 3.0f, 2.0f + (4.0f * s), 2.0f, 1.0f - s2;

        Eigen::VectorXf upper(4);
        upper << -1.0f, -1.0f, -1.0f, 0.0f;

        REQUIRE(matrices_are_equal(hessian.lower, lower, 4));
        REQUIRE(matrices_are_equal(hessian.center, center, 4));
        REQUIRE(matrices_are_equal(hessian.upper, upper, 4));
    }

    SECTION("Clamped ends") {
        char boundary_conditions[4];
        boundary_conditions[0] = EDGE_CLAMPED;
        boundary_conditions[1] = EDGE_STRESS_FREE;
        boundary_conditions[2] = EDGE_STRESS_FREE;
        boundary_conditions[3] = EDGE_CLAMPED;

        Hessian hessian = compute_hessian_d2Edtheta2(
            beta,
            neighbor_len_bar,
            omega,
            bending_modulus,
            omega_bar,
            boundary_conditions);

        REQUIRE(hessian.lower.size() == 4);
        REQUIRE(hessian.center.size() == 4);
        REQUIRE(hessian.upper.size() == 4);

        Eigen::VectorXf lower(4);
        lower << 0.0f, -1.0f, -1.0f, 0.0f;

        Eigen::VectorXf center(4);
        center << 1.0f, 2.0f + (4.0f * s), 2.0f, 1.0f;

        Eigen::VectorXf upper(4);
        upper << 0.0f, -1.0f, -1.0f, 0.0f;

        REQUIRE(matrices_are_equal(hessian.lower, lower, 4));
        REQUIRE(matrices_are_equal(hessian.center, center, 4));
        REQUIRE(matrices_are_equal(hessian.upper, upper, 4));
    }
}