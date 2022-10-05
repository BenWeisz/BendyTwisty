#pragma once

#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <catch2/catch_approx.hpp>

#include "../include/custom/rod/compute_skew_matrices.h"

TEST_CASE("Two random vectors", "[compute_skew_matrix]") {
    Eigen::Vector3f a = Eigen::Vector3f::Random();
    Eigen::Vector3f b = Eigen::Vector3f::Random();

    Eigen::Matrix3f skew = compute_skew_matrix(a);
    Eigen::Vector3f res = skew * b;

    Eigen::Vector3f truth = a.cross(b);

    REQUIRE(truth(0) == Catch::Approx(res(0)));
    REQUIRE(truth(1) == Catch::Approx(res(1)));
    REQUIRE(truth(2) == Catch::Approx(res(2)));
}

TEST_CASE("Vector with itself", "[compute_skew_matrix]") {
    Eigen::Vector3f a;
    a << 1, 1, 1;
    Eigen::Vector3f b;
    b << 1, 1, 1;

    Eigen::Matrix3f skew = compute_skew_matrix(a);
    Eigen::Vector3f res = skew * b;

    Eigen::Vector3f truth = a.cross(b);

    REQUIRE(truth(0) == Catch::Approx(res(0)));
    REQUIRE(truth(1) == Catch::Approx(res(1)));
    REQUIRE(truth(2) == Catch::Approx(res(2)));
}

TEST_CASE("Many skew matrices", "[compute_skew_matrices]") {
    Eigen::MatrixXf v(3, 4);
    v << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12;

    std::vector<Eigen::Matrix3f> skews = compute_skew_matrices(v);

    for (int i = 0; i < 3; i++) {
        Eigen::Matrix3f skew = skews[i];
        Eigen::Vector3f random_vec = Eigen::Vector3f::Random();

        Eigen::Vector3f res = skew * random_vec;
        Eigen::Vector3f truth = ((Eigen::Vector3f)v.col(i)).cross(random_vec);

        REQUIRE(truth(0) == Catch::Approx(res(0)));
        REQUIRE(truth(1) == Catch::Approx(res(1)));
        REQUIRE(truth(2) == Catch::Approx(res(2)));
    }
}