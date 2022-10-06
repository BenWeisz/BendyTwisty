#pragma once

#include <Eigen/Core>

#include "../include/custom/rod/compute_grad_holonomy.h"
#include "util.h"

TEST_CASE("Square Loop holonomy gradient", "[compute_grad_holonomy]") {
    Eigen::MatrixXf ebar(3, 4);
    ebar << 1, 0, -1, 0, 0, -1, 0, 1, 0, 0, 0, 0;

    Eigen::MatrixXf kb(3, 3);
    kb << 0, 0, 0, 0, 0, 0, -2, -2, -2;

    PsiGrad psi_grad = compute_grad_holonomy(kb, ebar);

    REQUIRE(psi_grad.psi_grad_im1.cols() == 3);
    REQUIRE(psi_grad.psi_grad_im1.rows() == 3);

    REQUIRE(psi_grad.psi_grad_ip1.cols() == 3);
    REQUIRE(psi_grad.psi_grad_ip1.rows() == 3);

    REQUIRE(psi_grad.psi_grad_i.cols() == 3);
    REQUIRE(psi_grad.psi_grad_i.rows() == 3);

    Eigen::MatrixXf im1 = Eigen::MatrixXf::Zero(3, 3);
    im1(2, 0) = -1.0f;
    im1(2, 1) = -1.0f;
    im1(2, 2) = -1.0f;

    Eigen::MatrixXf ip1 = Eigen::MatrixXf::Zero(3, 3);
    ip1(2, 0) = 1.0f;
    ip1(2, 1) = 1.0f;
    ip1(2, 2) = 1.0f;

    Eigen::MatrixXf i = Eigen::MatrixXf::Zero(3, 3);
    i(2, 0) = 0.0f;
    i(2, 1) = 0.0f;
    i(2, 2) = 0.0f;

    REQUIRE(matrices_are_equal(psi_grad.psi_grad_im1, im1, 9));
    REQUIRE(matrices_are_equal(psi_grad.psi_grad_ip1, ip1, 9));
    REQUIRE(matrices_are_equal(psi_grad.psi_grad_i, i, 9));
}

TEST_CASE("Square Loop holonomy gradient sum", "[compute_grad_holonomy_sum]") {
    Eigen::MatrixXf im1 = Eigen::MatrixXf::Zero(3, 3);
    im1(2, 0) = -1.0f;
    im1(2, 1) = -1.0f;
    im1(2, 2) = -1.0f;

    Eigen::MatrixXf ip1 = Eigen::MatrixXf::Zero(3, 3);
    ip1(2, 0) = 1.0f;
    ip1(2, 1) = 1.0f;
    ip1(2, 2) = 1.0f;

    Eigen::MatrixXf i = Eigen::MatrixXf::Zero(3, 3);
    i(2, 0) = 0.0f;
    i(2, 1) = 0.0f;
    i(2, 2) = 0.0f;

    PsiGrad psi_grad;
    psi_grad.psi_grad_im1 = im1;
    psi_grad.psi_grad_ip1 = ip1;
    psi_grad.psi_grad_i = i;

    PsiGradSum psi_grad_sum = compute_grad_holonomy_sum(psi_grad);

    REQUIRE(psi_grad_sum.size() == 5);

    Eigen::MatrixXf grad0 = Eigen::MatrixXf::Zero(3, 3);
    grad0(2, 0) = -1.0f;
    grad0(2, 1) = -1.0f;
    grad0(2, 2) = -1.0f;

    Eigen::MatrixXf grad1 = Eigen::MatrixXf::Zero(3, 3);
    grad1(2, 1) = -1.0f;
    grad1(2, 2) = -1.0f;

    Eigen::MatrixXf grad2 = Eigen::MatrixXf::Zero(3, 3);
    grad2(2, 0) = 1.0f;
    grad2(2, 1) = 1.0f;

    Eigen::MatrixXf grad4 = Eigen::MatrixXf::Zero(3, 3);
    grad4(2, 2) = 1.0f;

    REQUIRE(matrices_are_equal(psi_grad_sum[0], grad0, 9));
    REQUIRE(matrices_are_equal(psi_grad_sum[1], grad1, 9));
    REQUIRE(matrices_are_equal(psi_grad_sum[2], grad2, 9));
    REQUIRE(matrices_are_equal(psi_grad_sum[3], -grad1, 9));
    REQUIRE(matrices_are_equal(psi_grad_sum[4], grad4, 9));
}