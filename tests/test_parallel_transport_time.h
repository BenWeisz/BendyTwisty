#pragma once

#include <vector>

#include <Eigen/Core>

#include "../include/custom/rod/parallel_transport_time.h"
#include "util.h"

TEST_CASE("Varying rod centerline", "[parallel_transport_time]") {
    Eigen::Matrix3f frame;
    frame << 1, 0, 0, 0, 0, 1, 0, -1, 0;

    // Set up the list of the previous frames
    std::vector<Eigen::Matrix3f> prev_frames;
    prev_frames.push_back(frame);
    prev_frames.push_back(frame);
    prev_frames.push_back(frame);

    // Set up the new edges
    Eigen::MatrixXf e(3, 3);
    e << 1, 1, 1,
        -1, -1, -1,
        0, 0, 0;

    // Compute the time tranported frames
    std::vector<Eigen::Matrix3f> new_frames = parallel_transport_time(prev_frames, e);

    float sqrt22 = 0.7071067f;
    Eigen::Matrix3f new_frame;
    new_frame << sqrt22, 0, sqrt22,
        -sqrt22, 0, sqrt22,
        0, -1, 0;

    REQUIRE(matrices_are_equal(new_frames[0], new_frame, 9));
    REQUIRE(matrices_are_equal(new_frames[1], new_frame, 9));
    REQUIRE(matrices_are_equal(new_frames[2], new_frame, 9));
}