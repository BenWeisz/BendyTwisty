#pragma once

#include <Eigen/Core>

#include "compute_elastic_forces.h"

void time_integrate(Eigen::MatrixXf& x, Eigen::MatrixXf& xbar, FORCE& force, const float mass, const float dt) {
    int num_segments = (force.size() / 3) - 1;
    Eigen::MatrixXf force_mat = Eigen::MatrixXf::Zero(3, num_segments + 1);

    for (int i = 0; i < num_segments + 1; i++) {
        force_mat.col(i) = force.segment(i * 3, 3);
    }

    xbar = xbar + ((dt / mass) * force_mat);
    x = x + (dt * xbar);
}