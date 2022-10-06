#pragma once

#include <cmath>
#include <vector>

#include <Eigen/Core>
#include "../include/custom/rod/compute_grad_kb.h"
#include "../include/custom/rod/compute_grad_omega.h"
#include "../include/custom/rod/compute_omega.h"
#include "util.h"

TEST_CASE("Square loop omega grad", "[compute_grad_omega]") {
    // Set up material frame
    // Note: the column vectors should have been normalized
    Eigen::Matrix3f mf0;
    mf0 << 1, 0, -1,
        1, 0, 1,
        0, -1, 0;
    Eigen::Matrix3f mf1;
    mf1 << 0, 1, 0,
        1, 0, 1,
        1, 0, -1;
    Eigen::Matrix3f mf2;
    mf2 << -1, 0, 1,
        1, 0, 1,
        0, 1, 0;
    Eigen::Matrix3f mf3;
    mf3 << 0, -1, 0,
        1, 0, 1,
        -1, 0, 1;

    std::vector<Eigen::Matrix3f> mf;
    mf.push_back(mf0);
    mf.push_back(mf1);
    mf.push_back(mf2);
    mf.push_back(mf3);

    // Set up kb_grad
    KBGrad kb_grad;
    float n = 9.0f / 2.0f;
    float s = 7.0f / 2.0f;
    float f = 5.0f / 2.0f;
    float t = 3.0f / 2.0f;
    float o = 1.0f / 2.0f;

    Eigen::MatrixXf zero = Eigen::MatrixXf::Zero(2, 3);

    Eigen::Matrix3f zero_one;
    zero_one << 0, -o, t, 1, -o, -o, -1, o, o;
    Eigen::Matrix3f one_two;
    one_two << o, -o, 1, o, -o, 1, -t, -o, 0;
    Eigen::Matrix3f two_three;
    two_three << 0, o, t, -1, -o, o, -1, -o, o;
    Eigen::Matrix3f two_one;
    two_one << -o, -o, 1, o, o, -1, -t, o, 0;
    Eigen::Matrix3f three_two;
    three_two << 0, -o, t, 1, o, o, -1, -o, -o;
    Eigen::Matrix3f four_three;
    four_three << -o, o, 1, -o, o, 1, -t, -o, 0;
    Eigen::Matrix3f one_one;
    one_one << o, 1, -f, -t, 0, t, f, -1, -o;
    Eigen::Matrix3f two_two;
    two_two << -o, 1, -f, -t, 0, -t, f, 1, o;
    Eigen::Matrix3f three_three;
    three_three << o, -1, -f, t, 0, -t, f, 1, -o;

    kb_grad.grad_kb_im1.push_back(zero_one);
    kb_grad.grad_kb_im1.push_back(one_two);
    kb_grad.grad_kb_im1.push_back(two_three);
    kb_grad.grad_kb_ip1.push_back(two_one);
    kb_grad.grad_kb_ip1.push_back(three_two);
    kb_grad.grad_kb_ip1.push_back(four_three);
    kb_grad.grad_kb_i.push_back(one_one);
    kb_grad.grad_kb_i.push_back(two_two);
    kb_grad.grad_kb_i.push_back(three_three);

    Omega omega;
    omega.omega_j_im1 = Eigen::MatrixXf(2, 3);
    omega.omega_j_im1 << -2, -2, -2, 1, 1, 1;

    omega.omega_j_i = Eigen::MatrixXf(2, 3);
    omega.omega_j_i << -2, -2, -2, -1, -1, -1;

    PsiGradSum psi_grad_sum;
    Eigen::MatrixXf psi_grad0(3, 3);
    psi_grad0 << 0, 0, 0, 0, 0, 0, -1, -1, -1;
    Eigen::MatrixXf psi_grad1(3, 3);
    psi_grad1 << 0, 0, 0, 0, 0, 0, 0, -1, -1;
    Eigen::MatrixXf psi_grad2(3, 3);
    psi_grad2 << 0, 0, 0, 0, 0, 0, 1, 1, 0;
    Eigen::MatrixXf psi_grad3(3, 3);
    psi_grad3 << 0, 0, 0, 0, 0, 0, 0, 1, 1;
    Eigen::MatrixXf psi_grad4(3, 3);
    psi_grad4 << 0, 0, 0, 0, 0, 0, 0, 0, 1;
    psi_grad_sum.push_back(psi_grad0);
    psi_grad_sum.push_back(psi_grad1);
    psi_grad_sum.push_back(psi_grad2);
    psi_grad_sum.push_back(psi_grad3);
    psi_grad_sum.push_back(psi_grad4);

    OmegaGrad omega_grad = compute_grad_omega(mf, kb_grad, omega, psi_grad_sum);

    Eigen::MatrixXf zero_zero_one(2, 3);
    zero_zero_one << 1, 0, -2, -1, o, o;
    Eigen::MatrixXf one_zero_one(2, 3);
    one_zero_one << -2, -1, 4, f, -1, -o;
    Eigen::MatrixXf two_zero_one(2, 3);
    two_zero_one << 1, 1, -2, -t, o, 0;
    Eigen::MatrixXf three_zero_one = zero;
    Eigen::MatrixXf four_zero_one = zero;

    Eigen::MatrixXf zero_one_two(2, 3);
    zero_one_two << 0, 0, -1, 0, 0, -2;
    Eigen::MatrixXf one_one_two(2, 3);
    one_one_two << 2, 0, 1, -o, o, -1;
    Eigen::MatrixXf two_one_two(2, 3);
    two_one_two << -4, -1, -1, o, -1, n;
    Eigen::MatrixXf three_one_two(2, 3);
    three_one_two << 2, 1, 1, 0, o, -t;
    Eigen::MatrixXf four_one_two = zero;

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

    Eigen::MatrixXf zero_one_one(2, 3);
    zero_one_one << 2, -1, 0, 0, o, -s;
    Eigen::MatrixXf one_one_one(2, 3);
    one_one_one << -4, 1, 2, -o, -1, f;
    Eigen::MatrixXf two_one_one(2, 3);
    two_one_one << 2, 0, -2, o, o, 1;
    Eigen::MatrixXf three_one_one = zero;
    Eigen::MatrixXf four_one_one = zero;

    Eigen::MatrixXf zero_two_two(2, 3);
    zero_two_two << 0, 0, 1, 0, 0, -2;
    Eigen::MatrixXf one_two_two(2, 3);
    one_two_two << 1, -1, 3, t, o, -2;
    Eigen::MatrixXf two_two_two(2, 3);
    two_two_two << -2, 1, -5, -f, -1, t;
    Eigen::MatrixXf three_two_two(2, 3);
    three_two_two << 1, 0, 1, 1, o, f;
    Eigen::MatrixXf four_two_two = zero;

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

    OmegaGrad_K omega_grad_k1 = omega_grad[0];
    OmegaGrad_K omega_grad_k2 = omega_grad[1];
    OmegaGrad_K omega_grad_k3 = omega_grad[2];

    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_km1[0], zero_zero_one, 6));
    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_km1[1], one_zero_one, 6));
    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_km1[2], two_zero_one, 6));
    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_km1[3], three_zero_one, 6));
    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_km1[4], four_zero_one, 6));

    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_km1[0], zero_one_two, 6));
    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_km1[1], one_one_two, 6));
    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_km1[2], two_one_two, 6));
    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_km1[3], three_one_two, 6));
    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_km1[4], four_one_two, 6));

    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_km1[0], zero_two_three, 6));
    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_km1[1], one_two_three, 6));
    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_km1[2], two_two_three, 6));
    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_km1[3], three_two_three, 6));
    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_km1[4], four_two_three, 6));

    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_k[0], zero_one_one, 6));
    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_k[1], one_one_one, 6));
    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_k[2], two_one_one, 6));
    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_k[3], three_one_one, 6));
    REQUIRE(matrices_are_equal(omega_grad_k1.omega_grad_j_k[4], four_one_one, 6));

    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_k[0], zero_two_two, 6));
    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_k[1], one_two_two, 6));
    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_k[2], two_two_two, 6));
    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_k[3], three_two_two, 6));
    REQUIRE(matrices_are_equal(omega_grad_k2.omega_grad_j_k[4], four_two_two, 6));

    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_k[0], zero_three_three, 6));
    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_k[1], one_three_three, 6));
    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_k[2], two_three_three, 6));
    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_k[3], three_three_three, 6));
    REQUIRE(matrices_are_equal(omega_grad_k3.omega_grad_j_k[4], four_three_three, 6));
}