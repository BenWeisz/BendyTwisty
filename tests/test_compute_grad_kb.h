#pragma once

#include <vector>
#include <Eigen/Core>

#include "../include/custom/rod/compute_grad_kb.h"
#include "util.h"

TEST_CASE("Square coil", "[compute_grad_kb]") {
    float f = 5.0f / 2.0f;
    float t = 3.0f / 2.0f;
    float o = 1.0f / 2.0f;

    Eigen::Matrix3f skew0;
    skew0 << 0, 0, 1, 0, 0, -1, -1, 1, 0;

    Eigen::Matrix3f skew1;
    skew1 << 0, -1, 1, 1, 0, 0, -1, 0, 0;

    Eigen::Matrix3f skew2;
    skew2 << 0, 0, 1, 0, 0, 1, -1, -1, 0;

    Eigen::Matrix3f skew3;
    skew3 << 0, 1, 1, -1, 0, 0, -1, 0, 0;

    std::vector<Eigen::Matrix3f> skews;
    skews.push_back(skew0);
    skews.push_back(skew1);
    skews.push_back(skew2);
    skews.push_back(skew3);

    Eigen::MatrixXf kb(3, 3);
    kb << 1, -1, -1, -1, -1, -1, 1, 1, -1;

    Eigen::MatrixXf e(3, 4);
    e << 1, 0, -1, 0, 1, 1, 1, 1, 0, 1, 0, -1;

    Eigen::MatrixXf ebar(3, 4);
    ebar << 1, 0, -1, 0, 0, 0, 0, 0, 0, 1, 0, -1;

    KBGrad kb_grad = compute_grad_kb(skews, kb, e, ebar);

    Eigen::Matrix3f zero_one;
    zero_one << 0, -o, t, 1, -o, -o, -1, o, o;

    Eigen::Matrix3f one_two;
    one_two << o, -o, 1, o, -o, 1, -t, -o, 0;

    Eigen::Matrix3f two_three;
    two_three << 0, o, t, -1, -o, o, -1, -o, o;

    REQUIRE(matrices_are_equal(kb_grad.grad_kb_im1[0], zero_one, 9));
    REQUIRE(matrices_are_equal(kb_grad.grad_kb_im1[1], one_two, 9));
    REQUIRE(matrices_are_equal(kb_grad.grad_kb_im1[2], two_three, 9));

    Eigen::Matrix3f two_one;
    two_one << -o, -o, 1, o, o, -1, -t, o, 0;

    Eigen::Matrix3f three_two;
    three_two << 0, -o, t, 1, o, o, -1, -o, -o;

    Eigen::Matrix3f four_three;
    four_three << -o, o, 1, -o, o, 1, -t, -o, 0;

    REQUIRE(matrices_are_equal(kb_grad.grad_kb_ip1[0], two_one, 9));
    REQUIRE(matrices_are_equal(kb_grad.grad_kb_ip1[1], three_two, 9));
    REQUIRE(matrices_are_equal(kb_grad.grad_kb_ip1[2], four_three, 9));

    Eigen::Matrix3f one_one;
    one_one << o, 1, -f, -t, 0, t, f, -1, -o;

    Eigen::Matrix3f two_two;
    two_two << -o, 1, -f, -t, 0, -t, f, 1, o;

    Eigen::Matrix3f three_three;
    three_three << o, -1, -f, t, 0, -t, f, 1, -o;

    REQUIRE(matrices_are_equal(kb_grad.grad_kb_i[0], one_one, 9));
    REQUIRE(matrices_are_equal(kb_grad.grad_kb_i[1], two_two, 9));
    REQUIRE(matrices_are_equal(kb_grad.grad_kb_i[2], three_three, 9));
}