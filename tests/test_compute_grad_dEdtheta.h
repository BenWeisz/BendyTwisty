#pragma once

#include <cmath>

#include <Eigen/Core>
#include <catch2/catch_approx.hpp>

#include "../include/custom/rod/compute_omega.h"
#include "../include/custom/rod/compute_grad_dEdtheta.h"
#include "../include/custom/rod/util.h"

TEST_CASE("Straight twisted rod", "[compute_grad_dEdtheta]") {
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

        Eigen::VectorXf grad = compute_grad_dEdtheta(
            neighbor_len_bar,
            omega,
            bending_modulus,
            omega_bar,
            beta,
            theta,
            boundary_conditions);

        REQUIRE(grad.size() == 4);
        REQUIRE(grad(0) == Catch::Approx(-M_PI / 4.0f));
        REQUIRE(grad(1) == Catch::Approx(4.0f * s));
        REQUIRE(grad(2) == Catch::Approx(4.0f));
        REQUIRE(grad(3) == Catch::Approx(s2 + (M_PI / 4.0f)));
    }

    SECTION("Clamped ends") {
        char boundary_conditions[4];
        boundary_conditions[0] = VERTEX_CLAMPED;
        boundary_conditions[1] = VERTEX_STRESS_FREE;
        boundary_conditions[2] = VERTEX_STRESS_FREE;
        boundary_conditions[3] = VERTEX_CLAMPED;

        Eigen::VectorXf grad = compute_grad_dEdtheta(
            neighbor_len_bar,
            omega,
            bending_modulus,
            omega_bar,
            beta,
            theta,
            boundary_conditions);

        REQUIRE(grad.size() == 4);
        REQUIRE(grad(0) == Catch::Approx(0));
        REQUIRE(grad(1) == Catch::Approx(4.0f * s));
        REQUIRE(grad(2) == Catch::Approx(4.0f));
        REQUIRE(grad(3) == Catch::Approx(0));
    }
}