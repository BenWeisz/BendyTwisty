#pragma once

#include <Eigen/Core>
#include <vector>

#include "../include/custom/rod/compute_material_frame.h"
#include "util.h"

TEST_CASE("Straight Rod", "[compute_material_frame]") {
    Eigen::Matrix3f u0;
    u0 << 1, 0, 0, 0, 0, 1, 0, -1, 0;

    std::vector<Eigen::Matrix3f> bf;
    bf.push_back(u0);
    bf.push_back(u0);
    bf.push_back(u0);
    bf.push_back(u0);

    Eigen::VectorXf theta = Eigen::VectorXf::Zero(4);
    theta(1) = M_PI / 4;
    theta(2) = M_PI / 2;
    theta(3) = (3 * M_PI) / 4;

    std::vector<Eigen::Matrix3f> mf = compute_material_frame(bf, theta);

    REQUIRE(mf.size() == 4);

    float sin_pi_4 = sin(M_PI / 4.0f);  // is also the same for cos

    Eigen::Matrix3f m1;
    m1 << 1, 0, 0, 0, sin_pi_4, sin_pi_4, 0, -sin_pi_4, sin_pi_4;

    Eigen::Matrix3f m2;
    m2 << 1, 0, 0, 0, 1, 0, 0, 0, 1;

    Eigen::Matrix3f m3;
    m3 << 1, 0, 0, 0, sin_pi_4, -sin_pi_4, 0, sin_pi_4, sin_pi_4;

    REQUIRE(mf.size() == 4);
    REQUIRE(frames_are_equal(mf[0], u0));
    REQUIRE(frames_are_equal(mf[1], m1));
    REQUIRE(frames_are_equal(mf[2], m2));
    REQUIRE(frames_are_equal(mf[3], m3));
}

TEST_CASE("Square loop and rotated", "[compute_material_frame]") {
    Eigen::Matrix3f u0;
    u0 << 1, 0, 0, 0, 0, 1, 0, -1, 0;

    Eigen::Matrix3f u1;
    u1 << 0, 0, 1, -1, 0, 0, 0, -1, 0;

    Eigen::Matrix3f u2;
    u2 << -1, 0, 0, 0, 0, -1, 0, -1, 0;

    Eigen::Matrix3f u3;
    u3 << 0, 0, -1, 1, 0, 0, 0, -1, 0;

    std::vector<Eigen::Matrix3f> bf;
    bf.push_back(u0);
    bf.push_back(u1);
    bf.push_back(u2);
    bf.push_back(u3);

    Eigen::VectorXf theta = Eigen::VectorXf::Zero(4);
    theta(1) = M_PI / 4;
    theta(2) = M_PI / 2;
    theta(3) = (3 * M_PI) / 4;

    std::vector<Eigen::Matrix3f> mf = compute_material_frame(bf, theta);

    REQUIRE(mf.size() == 4);

    float s = sin(M_PI / 4.0f);  // is also the same for cos

    Eigen::Matrix3f m1;
    m1 << 0, s, s, -1, 0, 0, 0, -s, s;

    Eigen::Matrix3f m2;
    m2 << -1, 0, 0, 0, -1, 0, 0, 0, 1;

    Eigen::Matrix3f m3;
    m3 << 0, -s, s, 1, 0, 0, 0, s, s;

    REQUIRE(mf.size() == 4);
    REQUIRE(frames_are_equal(mf[0], u0));
    REQUIRE(frames_are_equal(mf[1], m1));
    REQUIRE(frames_are_equal(mf[2], m2));
    REQUIRE(frames_are_equal(mf[3], m3));
}