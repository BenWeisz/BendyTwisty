#pragma once

#include <Eigen/Core>
#include "../include/custom/rod/compute_pdE_pdtheta.h"
#include "../include/custom/rod/compute_omega.h"
#include "util.h"

TEST_CASE("Test coil dpEdptheta", "[compute_pdE_pdtheta]") {
    // Set up the neighbourhood lengths
    Eigen::VectorXf neighbor_len_bar(3);
    neighbor_len_bar(0) = 2;
    neighbor_len_bar(1) = 2;
    neighbor_len_bar(2) = 2;

    // Set up the bending modules beta * I2
    float beta = 2.0f;
    Eigen::Matrix2f bending_modulus = Eigen::Matrix2f::Identity() * beta;

    // Set up the initial and current omega for the rod
    Omega omega;
    omega.omega_j_im1 = Eigen::MatrixXf(2, 3);
    omega.omega_j_im1 << -2, -2, -2, 1, 1, 1;

    omega.omega_j_i = Eigen::MatrixXf(2, 3);
    omega.omega_j_i << -2, -2, -2, -1, -1, -1;

    Omega omega_bar;
    omega_bar.omega_j_im1 = Eigen::MatrixXf(2, 3);
    omega_bar.omega_j_im1 << -1, -1, -1, 0, 0, 0;

    omega_bar.omega_j_i = Eigen::MatrixXf(2, 3);
    omega_bar.omega_j_i << -1, -1, -1, 0, 0, 0;

    Eigen::VectorXf theta(4);
    theta << 1, 2, 3, 4;

    Eigen::VectorXf dEdtheta = compute_pdE_pdtheta(neighbor_len_bar, omega, omega_bar, bending_modulus, beta, theta);

    REQUIRE(matrices_are_equal(dEdtheta, Eigen::Vector3f::Zero(), 3));
}