#pragma once

#include <Eigen/Core>

Eigen::VectorXf compute_neighbor_len(Eigen::MatrixXf& e) {
    int num_segments = e.cols();

    Eigen::VectorXf emag = (e.transpose() * e).diagonal().cwiseSqrt();
    Eigen::VectorXf neighbor_len = emag.segment(0, num_segments - 1) + emag.segment(1, num_segments - 1);

    return neighbor_len;
}