#pragma once

#include <vector>

#include <Eigen/Core>

std::vector<Eigen::Matrix3f> compute_material_frame(std::vector<Eigen::Matrix3f>& bf, Eigen::VectorXf& theta) {
    int num_segments = bf.size();
    std::vector<Eigen::Matrix3f> material_frames;

    Eigen::VectorXf cos_theta = theta.array().cos();
    Eigen::VectorXf sin_theta = theta.array().sin();

    for (int i = 0; i < num_segments; i++) {
        Eigen::Vector3f u = bf[i].col(1);
        Eigen::Vector3f v = bf[i].col(2);

        Eigen::Vector3f m1 = (cos_theta[i] * u) + (sin_theta[i] * v);
        Eigen::Vector3f m2 = (-sin_theta[i] * u) + (cos_theta[i] * v);

        Eigen::Matrix3f frame;
        frame.col(0) = bf[i].col(0);
        frame.col(1) = m1;
        frame.col(2) = m2;

        material_frames.push_back(frame);
    }

    return material_frames;
}