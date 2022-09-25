#include <Eigen/Core>

Eigen::VectorXf compute_phi(Eigen::MatrixXf& e) {
    int num_segments = e.cols();
    Eigen::MatrixXf eim1 = e.block(0, 0, 3, num_segments - 1);
    Eigen::MatrixXf ei = e.block(0, 1, 3, num_segments - 1);

    auto top = (eim1.transpose() * ei).diagonal();

    auto eim1_mag = (eim1.transpose() * eim1).diagonal().cwiseSqrt();
    auto ei_mag = (ei.transpose() * ei).diagonal().cwiseSqrt();
    Eigen::VectorXf bottom = eim1_mag.cwiseProduct(ei_mag);

    Eigen::VectorXf cos_phi = top.array() / bottom.array();
    Eigen::VectorXf phi = cos_phi.array().acos();
    return phi;
}