#pragma once

#include <cmath>

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <catch2/catch_approx.hpp>

#include "../include/custom/rod/compute_omega.h"
#include "../include/custom/rod/compute_hessian_d2Edtheta2.h"
#include "../include/custom/rod/util.h"

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
        boundary_conditions[0] = VERTEX_STRESS_FREE;
        boundary_conditions[1] = VERTEX_STRESS_FREE;
        boundary_conditions[2] = VERTEX_STRESS_FREE;
        boundary_conditions[3] = VERTEX_STRESS_FREE;

        Eigen::SparseMatrix<float> hessian = compute_hessian_d2Edtheta2(
            beta,
            neighbor_len_bar,
            omega,
            bending_modulus,
            omega_bar,
            boundary_conditions);

        REQUIRE(hessian.cols() == 4);
        REQUIRE(hessian.rows() == 4);

        REQUIRE(hessian.coeff(0, 0) == 3.0f);
        REQUIRE(hessian.coeff(0, 1) == -1.0f);
        REQUIRE(hessian.coeff(0, 2) == 0.0f);
        REQUIRE(hessian.coeff(0, 3) == 0.0f);

        REQUIRE(hessian.coeff(1, 0) == -1.0f);
        REQUIRE(hessian.coeff(1, 1) == Catch::Approx(2 + (4 * s)));
        REQUIRE(hessian.coeff(1, 2) == -1.0f);
        REQUIRE(hessian.coeff(1, 3) == 0.0f);

        REQUIRE(hessian.coeff(2, 0) == 0.0f);
        REQUIRE(hessian.coeff(2, 1) == -1.0f);
        REQUIRE(hessian.coeff(2, 2) == 2.0f);
        REQUIRE(hessian.coeff(2, 3) == -1.0f);

        REQUIRE(hessian.coeff(3, 0) == 0.0f);
        REQUIRE(hessian.coeff(3, 1) == 0.0f);
        REQUIRE(hessian.coeff(3, 2) == -1.0f);
        REQUIRE(hessian.coeff(3, 3) == Catch::Approx(1.0f - s2));
    }

    SECTION("Clamped ends") {
        char boundary_conditions[4];
        boundary_conditions[0] = VERTEX_CLAMPED;
        boundary_conditions[1] = VERTEX_STRESS_FREE;
        boundary_conditions[2] = VERTEX_STRESS_FREE;
        boundary_conditions[3] = VERTEX_CLAMPED;

        Eigen::SparseMatrix<float> hessian = compute_hessian_d2Edtheta2(
            beta,
            neighbor_len_bar,
            omega,
            bending_modulus,
            omega_bar,
            boundary_conditions);

        REQUIRE(hessian.cols() == 4);
        REQUIRE(hessian.rows() == 4);

        REQUIRE(hessian.coeff(0, 0) == 0.0f);
        REQUIRE(hessian.coeff(0, 1) == 0.0f);
        REQUIRE(hessian.coeff(0, 2) == 0.0f);
        REQUIRE(hessian.coeff(0, 3) == 0.0f);

        REQUIRE(hessian.coeff(1, 0) == -1.0f);
        REQUIRE(hessian.coeff(1, 1) == Catch::Approx(2 + (4 * s)));
        REQUIRE(hessian.coeff(1, 2) == -1.0f);
        REQUIRE(hessian.coeff(1, 3) == 0.0f);

        REQUIRE(hessian.coeff(2, 0) == 0.0f);
        REQUIRE(hessian.coeff(2, 1) == -1.0f);
        REQUIRE(hessian.coeff(2, 2) == 2.0f);
        REQUIRE(hessian.coeff(2, 3) == -1.0f);

        REQUIRE(hessian.coeff(3, 0) == 0.0f);
        REQUIRE(hessian.coeff(3, 1) == 0.0f);
        REQUIRE(hessian.coeff(3, 2) == 0.0f);
        REQUIRE(hessian.coeff(3, 3) == 0.0f);
    }
}