#pragma once

#include <vector>
#include <cmath>

#include <Eigen/Core>
#include <catch2/catch_approx.hpp>

#include "../include/custom/rod/compute_omega.h"
#include "util.h"

TEST_CASE("Compute omega for twisted square loop", "[compute_omega]") {
    Eigen::MatrixXf kb = Eigen::MatrixXf::Zero(3, 3);
    kb(2, 0) = -2;
    kb(2, 1) = -2;
    kb(2, 2) = -2;

    float s = sin(M_PI / 4.0f);  // is also the same for cos

    Eigen::Matrix3f m0;
    m0 << 1, 0, 0, 0, 0, 1, 0, -1, 0;

    Eigen::Matrix3f m1;
    m1 << 0, s, s, -1, 0, 0, 0, -s, s;

    Eigen::Matrix3f m2;
    m2 << -1, 0, 0, 0, -1, 0, 0, 0, 1;

    Eigen::Matrix3f m3;
    m3 << 0, -s, s, 1, 0, 0, 0, s, s;

    std::vector<Eigen::Matrix3f> mf;
    mf.push_back(m0);
    mf.push_back(m1);
    mf.push_back(m2);
    mf.push_back(m3);

    Omega omega = compute_omega(kb, mf);

    float s2 = s * 2;

    Eigen::MatrixXf im1(2, 3);
    im1 << 0, -s2, -2, -2, -s2, 0;

    Eigen::MatrixXf i(2, 3);
    i << -s2, -2, -s2, -s2, 0, s2;

    REQUIRE(matrices_are_equal(omega.omega_j_im1, im1, 6));
    REQUIRE(matrices_are_equal(omega.omega_j_i, i, 6));
}