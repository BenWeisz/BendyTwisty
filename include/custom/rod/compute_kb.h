#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

Eigen::MatrixXf compute_kb(Eigen::MatrixXf &e, Eigen::MatrixXf &ebar) {
    int num_segments = e.cols();

    Eigen::MatrixXf eim1 = e.block(0, 0, 3, num_segments - 1);
    Eigen::MatrixXf ei = e.block(0, 1, 3, num_segments - 1);

    // Bottom term: |ebar_i-1| * |ebar_i| + e_i-1 . ei
    auto ebar_mag = (ebar.transpose() * ebar).diagonal().cwiseSqrt();
    auto ebar_mag_prod = ebar_mag.segment(0, num_segments - 1)
                             .cwiseProduct(ebar_mag.segment(1, num_segments - 1));
    auto e_dot = (eim1.transpose() * ei).diagonal();
    auto bottom = ebar_mag_prod + e_dot;

    // Top term: 2 * (e_i-1 x e_i)
    Eigen::MatrixXf kb = Eigen::MatrixXf::Zero(3, num_segments - 1);
    for (int i = 0; i < num_segments - 1; i++) {
        Eigen::Vector3f e1 = eim1.col(i);
        Eigen::Vector3f e2 = ei.col(i);

        kb.col(i) = (2.0 * e1.cross(e2)) / bottom(i);
    }

    return kb;
}
