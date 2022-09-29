#pragma once

#include <vector>
#include <iostream>

#include <Eigen/Core>

typedef struct KBGrad {
    std::vector<Eigen::Matrix3f> grad_kb_im1;
    std::vector<Eigen::Matrix3f> grad_kb_i;
    std::vector<Eigen::Matrix3f> grad_kb_ip1;
} KBGrad;

KBGrad compute_grad_kb(std::vector<Eigen::Matrix3f>& e_skew_matrices, Eigen::MatrixXf& kb, Eigen::MatrixXf& e, Eigen::MatrixXf& ebar) {
    int num_segments = e.cols();

    Eigen::MatrixXf eim1 = e.block(0, 0, 3, num_segments - 1);
    Eigen::MatrixXf ei = e.block(0, 1, 3, num_segments - 1);

    // Bottom term: |ebar_i-1| * |ebar_i| + e_i-1 . ei
    auto ebar_mag = (ebar.transpose() * ebar).diagonal().cwiseSqrt();
    auto ebar_mag_prod = ebar_mag.segment(0, num_segments - 1)
                             .cwiseProduct(ebar_mag.segment(1, num_segments - 1));
    auto e_dot = (eim1.transpose() * ei).diagonal();
    auto bottom = ebar_mag_prod + e_dot;

    KBGrad grad;

    for (int i = 0; i < num_segments - 1; i++) {
        Eigen::Matrix3f grad_im1 = ((e_skew_matrices[i + 1] * 2.0) + (kb.col(i) * e.col(i + 1).transpose())) / bottom(i);
        Eigen::Matrix3f grad_ip1 = ((e_skew_matrices[i] * 2.0) - (kb.col(i) * e.col(i).transpose())) / bottom(i);
        Eigen::Matrix3f grad_i = -(grad_im1 + grad_ip1);

        grad.grad_kb_im1.push_back(grad_im1);
        grad.grad_kb_i.push_back(grad_i);
        grad.grad_kb_ip1.push_back(grad_ip1);
    }

    return grad;
}