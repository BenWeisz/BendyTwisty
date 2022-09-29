#pragma once

#include <Eigen/Core>

// Take the position x and compute the edge vectors
Eigen::MatrixXf compute_edges(Eigen::MatrixXf &x) {
    int num_segments = x.cols() - 1;
    Eigen::MatrixXf edges =
        x.block(0, 1, 3, num_segments) - x.block(0, 0, 3, num_segments);

    return edges;
}