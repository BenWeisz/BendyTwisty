#pragma once

#define EDGE_STRESS_FREE 0
#define EDGE_CLAMPED 1

// Rotate 3d vector v about the unit axis k by angle theta
Eigen::Vector3f rotate_about_unit_axis(const Eigen::Vector3f& v, const Eigen::Vector3f& k, float theta) {
    Eigen::Vector3f term1 = v * cos(theta);
    Eigen::Vector3f term2 = k.cross(v) * sin(theta);
    Eigen::Vector3f term3 = k * (k.dot(v)) * (1 - cos(theta));
    return term1 + term2 + term3;
}