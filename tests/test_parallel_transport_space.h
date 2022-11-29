#pragma once

#include <vector>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <limits>

#include <Eigen/Core>

#include "../include/custom/rod/parallel_transport_space.h"
#include "util.h"

TEST_CASE("Rotate from axis to axis", "[rotate_about_unit_axis]") {
    Eigen::Vector3f v;
    v << 1, 0, 0;

    Eigen::Vector3f k;
    k << 0, 0, 1;

    Eigen::Vector3f vk = rotate_about_unit_axis(v, k, M_PI / 2);
    REQUIRE(std::abs(vk(0)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
    REQUIRE(vk(1) == Catch::Approx(1));
    REQUIRE(std::abs(vk(2)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
}

TEST_CASE("Rotate from plane to plane", "[rotate_about_unit_axis]") {
    Eigen::Vector3f v;
    v << 1, 1, 0;

    Eigen::Vector3f k;
    k << 1, 0, 0;

    Eigen::Vector3f vk = rotate_about_unit_axis(v, k, M_PI / 2);
    REQUIRE(vk(0) == Catch::Approx(1.0f));
    REQUIRE(std::abs(vk(1)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
    REQUIRE(vk(2) == Catch::Approx(1.0f));
}

TEST_CASE("Rotation preserves magnitude", "[rotate_about_unit_axis]") {
    Eigen::Vector3f v;
    v << 100, 0, 0;

    Eigen::Vector3f k;
    k << 0, 0, 1;

    Eigen::Vector3f vk = rotate_about_unit_axis(v, k, M_PI / 2);
    REQUIRE(std::abs(vk(0)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
    REQUIRE(vk(1) == Catch::Approx(100));
    REQUIRE(std::abs(vk(2)) == Catch::Approx(0.0f).margin(std::numeric_limits<float>::epsilon() * 100));
}

TEST_CASE("Rotation about zero vector is same as original", "[rotate_about_unit_axis]") {
    Eigen::Vector3f zero = Eigen::Vector3f::Zero();
    Eigen::Vector3f v;
    v << 1, 2, 3;

    Eigen::Vector3f vrot = rotate_about_unit_axis(v, zero, 0.0f);
    REQUIRE(v(0) == vrot(0));
    REQUIRE(v(1) == vrot(1));
    REQUIRE(v(2) == vrot(2));
}

TEST_CASE("Square Loop Rod", "[parallel_transport_space]") {
    Eigen::Matrix3f u0;
    u0 << 1, 0, 0, 0, 0, 1, 0, -1, 0;

    Eigen::Matrix3f u1;
    u1 << 0, 0, 1, -1, 0, 0, 0, -1, 0;

    Eigen::Matrix3f u2;
    u2 << -1, 0, 0, 0, 0, -1, 0, -1, 0;

    Eigen::Matrix3f u3;
    u3 << 0, 0, -1, 1, 0, 0, 0, -1, 0;

    Eigen::MatrixXf kb = Eigen::MatrixXf::Zero(3, 3);
    kb(2, 0) = -2;
    kb(2, 1) = -2;
    kb(2, 2) = -2;

    Eigen::VectorXf phi = Eigen::VectorXf::Ones(3).array() * (M_PI / 2.0f);
    std::vector<Eigen::Matrix3f> bishop_frames = parallel_transport_space(u0, kb, phi);

    REQUIRE(matrices_are_equal(bishop_frames[0], u0, 9));
    REQUIRE(matrices_are_equal(bishop_frames[1], u1, 9));
    REQUIRE(matrices_are_equal(bishop_frames[2], u2, 9));
    REQUIRE(matrices_are_equal(bishop_frames[3], u3, 9));
}

TEST_CASE("Straight Rod", "[parallel_transport_space]") {
    Eigen::Matrix3f u0;
    u0 << 1, 0, 0, 0, 0, 1, 0, -1, 0;

    Eigen::MatrixXf kb = Eigen::MatrixXf::Zero(3, 3);
    kb(2, 0) = -1;
    kb(2, 1) = -1;
    kb(2, 2) = -1;

    Eigen::VectorXf phi = Eigen::VectorXf::Zero(3);
    std::vector<Eigen::Matrix3f> bishop_frames = parallel_transport_space(u0, kb, phi);

    REQUIRE(bishop_frames.size() == 4);
    REQUIRE(matrices_are_equal(bishop_frames[0], u0, 9));
    REQUIRE(matrices_are_equal(bishop_frames[1], u0, 9));
    REQUIRE(matrices_are_equal(bishop_frames[2], u0, 9));
    REQUIRE(matrices_are_equal(bishop_frames[3], u0, 9));
}