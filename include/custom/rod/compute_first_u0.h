#pragma once

#include <Eigen/Core>

Eigen::Matrix3f compute_first_u0(Eigen::Vector3f& e) {
    Eigen::Vector3f t = e.normalized();
    
    // Use Gram-Schmidt to generate the remaining two basis vectors
    Eigen::Vector3f rand_u = Eigen::Vector3f::Random();
    Eigen::Vector3f proj_t = (rand_u.dot(t) / t.dot(t)) * t;

    Eigen::Vector3f u = rand_u - proj_t;
    u.normalize();

    // The final vector is just the cross product between the other two
    Eigen::Vector3f v = t.cross(u);
    v.normalize();

    Eigen::Matrix3f u0;
    u0.col(0) = t;
    u0.col(1) = u;
    u0.col(2) = v;

    return u0;
}