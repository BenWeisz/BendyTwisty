#pragma once

#include <vector>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <limits>

#include <Eigen/Core>

bool frames_are_equal(Eigen::Matrix3f& f1, Eigen::Matrix3f& f2) {
    bool res = true;
    for (int i = 0; i < 9; i++) {
        res = res && (f1.array()(i) == Catch::Approx(f2.array()(i)).margin(std::numeric_limits<float>::epsilon() * 100));
    }

    return res;
}