#pragma once

#include <Eigen/Core>
#include "../include/custom/rod/compute_pdE_pdx.h"
#include "../include/custom/rod/compute_grad_omega.h"
#include "../include/custom/rod/compute_omega.h"
#include "util.h"

TEST_CASE("Test coil dpEdpx", "[compute_pdE_pdx]") {
    // Set up the neighbourhood lengths
    Eigen::VectorXf neighbor_len_bar(3);
    neighbor_len_bar(0) = 2;
    neighbor_len_bar(1) = 2;
    neighbor_len_bar(2) = 2;

    // Set up the bending modules beta * I2
    Eigen::Matrix2f bending_modulus = Eigen::Matrix2f::Identity() * 2.0f;

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

    // Set up the omega_grad structure for the rod
    Eigen::MatrixXf zero = Eigen::MatrixXf::Zero(2, 3);
    float n = 9.0f / 2.0f;
    float s = 7.0f / 2.0f;
    float f = 5.0f / 2.0f;
    float t = 3.0f / 2.0f;
    float o = 1.0f / 2.0f;

    OmegaGrad_K_J omega_k1_km1;
    Eigen::MatrixXf zero_zero_one(2, 3);
    zero_zero_one << 1, 0, -2, -1, o, o;
    Eigen::MatrixXf one_zero_one(2, 3);
    one_zero_one << -2, -1, 4, f, -1, -o;
    Eigen::MatrixXf two_zero_one(2, 3);
    two_zero_one << 1, 1, -2, -t, o, 0;
    Eigen::MatrixXf three_zero_one = zero;
    Eigen::MatrixXf four_zero_one = zero;
    omega_k1_km1.push_back(zero_zero_one);
    omega_k1_km1.push_back(one_zero_one);
    omega_k1_km1.push_back(two_zero_one);
    omega_k1_km1.push_back(three_zero_one);
    omega_k1_km1.push_back(four_zero_one);

    OmegaGrad_K_J omega_k2_km1;
    Eigen::MatrixXf zero_one_two(2, 3);
    zero_one_two << 0, 0, -1, 0, 0, -2;
    Eigen::MatrixXf one_one_two(2, 3);
    one_one_two << 2, 0, 1, -o, o, -1;
    Eigen::MatrixXf two_one_two(2, 3);
    two_one_two << -4, -1, -1, o, -1, n;
    Eigen::MatrixXf three_one_two(2, 3);
    three_one_two << 2, 1, 1, 0, o, -t;
    Eigen::MatrixXf four_one_two = zero;
    omega_k2_km1.push_back(zero_one_two);
    omega_k2_km1.push_back(one_one_two);
    omega_k2_km1.push_back(two_one_two);
    omega_k2_km1.push_back(three_one_two);
    omega_k2_km1.push_back(four_one_two);

    OmegaGrad_K_J omega_k3_km1;
    Eigen::MatrixXf zero_two_three(2, 3);
    zero_two_three << 0, 0, -1, 0, 0, -2;
    Eigen::MatrixXf one_two_three(2, 3);
    one_two_three << 0, 0, -1, 0, 0, -2;
    Eigen::MatrixXf two_two_three(2, 3);
    two_two_three << -1, 0, 3, 1, o, t;
    Eigen::MatrixXf three_two_three(2, 3);
    three_two_three << 2, -1, -3, -f, -1, f;
    Eigen::MatrixXf four_two_three(2, 3);
    four_two_three << -1, 1, 2, t, o, 0;
    omega_k3_km1.push_back(zero_two_three);
    omega_k3_km1.push_back(one_two_three);
    omega_k3_km1.push_back(two_two_three);
    omega_k3_km1.push_back(three_two_three);
    omega_k3_km1.push_back(four_two_three);

    OmegaGrad_K_J omega_k1_k;
    Eigen::MatrixXf zero_one_one(2, 3);
    zero_one_one << 2, -1, 0, 0, o, -s;
    Eigen::MatrixXf one_one_one(2, 3);
    one_one_one << -4, 1, 2, -o, -1, f;
    Eigen::MatrixXf two_one_one(2, 3);
    two_one_one << 2, 0, -2, o, o, 1;
    Eigen::MatrixXf three_one_one = zero;
    Eigen::MatrixXf four_one_one = zero;
    omega_k1_k.push_back(zero_one_one);
    omega_k1_k.push_back(one_one_one);
    omega_k1_k.push_back(two_one_one);
    omega_k1_k.push_back(three_one_one);
    omega_k1_k.push_back(four_one_one);

    OmegaGrad_K_J omega_k2_k;
    Eigen::MatrixXf zero_two_two(2, 3);
    zero_two_two << 0, 0, 1, 0, 0, -2;
    Eigen::MatrixXf one_two_two(2, 3);
    one_two_two << 1, -1, 3, t, o, -2;
    Eigen::MatrixXf two_two_two(2, 3);
    two_two_two << -2, 1, -5, -f, -1, t;
    Eigen::MatrixXf three_two_two(2, 3);
    three_two_two << 1, 0, 1, 1, o, f;
    Eigen::MatrixXf four_two_two = zero;
    omega_k2_k.push_back(zero_two_two);
    omega_k2_k.push_back(one_two_two);
    omega_k2_k.push_back(two_two_two);
    omega_k2_k.push_back(three_two_two);
    omega_k2_k.push_back(four_two_two);

    OmegaGrad_K_J omega_k3_k;
    Eigen::MatrixXf zero_three_three(2, 3);
    zero_three_three << 0, 0, 1, 0, 0, -2;
    Eigen::MatrixXf one_three_three(2, 3);
    one_three_three << 0, 0, 1, 0, 0, -2;
    Eigen::MatrixXf two_three_three(2, 3);
    two_three_three << -2, -1, 1, 0, o, t;
    Eigen::MatrixXf three_three_three(2, 3);
    three_three_three << 4, 1, -3, o, -1, -o;
    Eigen::MatrixXf four_three_three(2, 3);
    four_three_three << -2, 0, 0, -o, o, 3;
    omega_k3_k.push_back(zero_three_three);
    omega_k3_k.push_back(one_three_three);
    omega_k3_k.push_back(two_three_three);
    omega_k3_k.push_back(three_three_three);
    omega_k3_k.push_back(four_three_three);

    OmegaGrad_K omega_grad_k1;
    omega_grad_k1.omega_grad_j_km1 = omega_k1_km1;
    omega_grad_k1.omega_grad_j_k = omega_k1_k;

    OmegaGrad_K omega_grad_k2;
    omega_grad_k2.omega_grad_j_km1 = omega_k2_km1;
    omega_grad_k2.omega_grad_j_k = omega_k2_k;

    OmegaGrad_K omega_grad_k3;
    omega_grad_k3.omega_grad_j_km1 = omega_k3_km1;
    omega_grad_k3.omega_grad_j_k = omega_k3_k;

    OmegaGrad omega_grad;
    omega_grad.push_back(omega_grad_k1);
    omega_grad.push_back(omega_grad_k2);
    omega_grad.push_back(omega_grad_k3);

    EPGradX e_grad_x = compute_pdE_pdx(neighbor_len_bar, omega_grad, bending_modulus, omega, omega_bar);

    Eigen::Vector3f dEdx0;
    dEdx0 << -4, 1, 6;

    Eigen::Vector3f dEdx1;
    dEdx1 << 4, 1, -12;

    Eigen::Vector3f dEdx2;
    dEdx2 << 8, 0, 8;

    Eigen::Vector3f dEdx3;
    dEdx3 << -13, -1, 3;

    Eigen::Vector3f dEdx4;
    dEdx4 << 5, -1, -5;

    REQUIRE(matrices_are_equal(e_grad_x[0], dEdx0, 3));
    REQUIRE(matrices_are_equal(e_grad_x[1], dEdx1, 3));
    REQUIRE(matrices_are_equal(e_grad_x[2], dEdx2, 3));
    REQUIRE(matrices_are_equal(e_grad_x[3], dEdx3, 3));
    REQUIRE(matrices_are_equal(e_grad_x[4], dEdx4, 3));
}