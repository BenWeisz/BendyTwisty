#include <vector>
#include <cmath>
#include <Eigen/Core>

// Rotate 3d vector v about the unit axis k by angle theta
Eigen::Vector3f rotate_about_unit_axis(const Eigen::Vector3f& v, const Eigen::Vector3f& k, float theta) {
    Eigen::Vector3f term1 = v * cos(theta);
    Eigen::Vector3f term2 = k.cross(v) * sin(theta);
    Eigen::Vector3f term3 = k * (k.dot(v)) * (1 - cos(theta));
    return term1 + term2 + term3;
}

// Parallel transport the given frame along the rod
std::vector<Eigen::Matrix3f> parallel_transport(Eigen::Matrix3f& f, Eigen::MatrixXf& kb, Eigen::VectorXf& phi) {
    int num_segments = kb.cols() + 1;
    std::vector<Eigen::Matrix3f> frames;

    Eigen::MatrixXf curr_frame = f;
    frames.push_back(curr_frame);
    for (int i = 0; i < num_segments - 1; i++) {
        Eigen::Vector3f k = kb.col(i).normalized();
        curr_frame.col(0) = rotate_about_unit_axis(curr_frame.col(0), k, phi(i));
        curr_frame.col(1) = rotate_about_unit_axis(curr_frame.col(1), k, phi(i));
        curr_frame.col(2) = rotate_about_unit_axis(curr_frame.col(2), k, phi(i));
        frames.push_back(curr_frame);
    }

    return frames;
}