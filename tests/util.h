#pragma once

#include <vector>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <limits>

#include <Eigen/Core>

bool matrices_are_equal(const Eigen::MatrixXf& f1, const Eigen::MatrixXf& f2, const int size) {
    bool res = true;
    for (int i = 0; i < size; i++) {
        res = res && (f1.array()(i) == Catch::Approx(f2.array()(i)).margin(std::numeric_limits<float>::epsilon() * 100));
    }

    return res;
}