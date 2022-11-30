#pragma once

#include <cmath>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "custom/rod/parallel_transport_space.h"

Eigen::Matrix3f compute_updated_u0(Eigen::MatrixXf& x, Eigen::Matrix3f& u0) {
    Eigen::Vector3f tt = u0.col(0);
    Eigen::Vector3f ut = u0.col(1);
    Eigen::Vector3f vt = u0.col(2);

    // Compute the tangent to the first edge at time t + 1
    Eigen::Vector3f ttp1 = x.col(1) - x.col(0);
    ttp1 = ttp1.normalized();

    // Compute the angle between the two vectors
    float phi = acos(tt.dot(ttp1));
    Eigen::Vector3f axis = tt.cross(ttp1);
    axis = axis.normalized();

    Eigen::Vector3f utp1 = rotate_about_unit_axis(ut, axis, phi);
    Eigen::Vector3f vtp1 = rotate_about_unit_axis(vt, axis, phi);

    // Set up the time transported frame
    Eigen::Matrix3f u0tp1;
    u0tp1.col(0) = ttp1;
    u0tp1.col(1) = utp1;
    u0tp1.col(2) = vtp1;

    return u0tp1;
}