#pragma once

#include <Eigen/Core>

// The strain that is used for enforcing the inextensibiliy constraint. ||e^i|| - ||e_bar^i|| = 0
Eigen::VectorXf compute_strain(Eigen::MatrixXf& e, Eigen::MatrixXf& ebar) {
    Eigen::VectorXf strain = (e.transpose() * e).diagonal() - (ebar.transpose() * ebar).diagonal();
    return strain;
}