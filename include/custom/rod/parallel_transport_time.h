#pragma once

#include <vector>
#include <cmath>

#include <Eigen/Core>

#include "util.h"

// Perform Time Parallel Transport of the existing bishop frames for each of the edges
/*
    prev_frames: List of 3x3 frame basis matrices for each of the edges
    e: Matrix of the new edges
*/
std::vector<Eigen::Matrix3f> parallel_transport_time(std::vector<Eigen::Matrix3f>& prev_frames, Eigen::MatrixXf& e) {
    int num_segments = e.cols();
    std::vector<Eigen::Matrix3f> new_frames;

    // Compute the edge lengths
    Eigen::VectorXf edge_len = (e.transpose() * e).diagonal().cwiseSqrt();
    Eigen::MatrixXf edge_unit = e.array().rowwise() / edge_len.transpose().array();

    // Parallel transport each frame forward in time
    for (int i = 0; i < num_segments; i++) {
        Eigen::Matrix3f new_frame = Eigen::Matrix3f::Zero();
        Eigen::Matrix3f prev_frame = prev_frames[i];

        // Compute the transport angle
        float angle = acos(prev_frame.col(0).dot(edge_unit.col(i)));

        // Compute the axis to rotate about
        Eigen::Vector3f axis = ((Eigen::Vector3f)(prev_frame.col(0))).cross((Eigen::Vector3f)edge_unit.col(i));
        axis.normalize();

        // Transport the previous frame in time
        new_frame.col(0) = edge_unit.col(i);
        new_frame.col(1) = rotate_about_unit_axis(prev_frame.col(1), axis, angle);
        new_frame.col(2) = rotate_about_unit_axis(prev_frame.col(2), axis, angle);

        new_frames.push_back(new_frame);
    }

    return new_frames;
}