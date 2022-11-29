#pragma once

#include <vector>
#include <cmath>

#include <Eigen/Core>

#include "util.h"

// Parallel transport the given frame along the rod
std::vector<Eigen::Matrix3f> parallel_transport_space(Eigen::Matrix3f& f, Eigen::MatrixXf& kb, Eigen::VectorXf& phi) {
    int num_segments = kb.cols() + 1;
    std::vector<Eigen::Matrix3f> frames;

    Eigen::MatrixXf curr_frame = f;
    frames.push_back(curr_frame);
    for (int i = 0; i < num_segments - 1; i++) {
        Eigen::Vector3f k = kb.col(i).normalized();
        curr_frame.col(0) = rotate_about_unit_axis(curr_frame.col(0), k, phi(i));
        curr_frame.col(1) = rotate_about_unit_axis(curr_frame.col(1), k, phi(i));
        curr_frame.col(2) = rotate_about_unit_axis(curr_frame.col(2), k, phi(i));
        frames.push_back(curr_frame);
    }

    return frames;
}