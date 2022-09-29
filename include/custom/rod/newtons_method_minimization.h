#pragma once

#include <vector>

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>

#include "compute_material_frame.h"
#include "compute_omega.h"
#include "compute_grad_dEdtheta.h"
#include "compute_hessian_d2Edtheta2.h"

Eigen::MatrixXf newtons_compute_omega(std::vector<Eigen::Matrix3f>& bf,
                                      Eigen::VectorXf& theta,
                                      Eigen::MatrixXf& kb,
                                      const char type) {
    // Compute Material Frame
    std::vector<Eigen::Matrix3f> mf = compute_material_frame(bf, theta);

    // Compute Omega
    Eigen::MatrixXf omega = compute_omega(kb, mf, type);
    return omega;
}

Eigen::VectorXf newtons_method_minimization(
    Eigen::VectorXf& neighbor_len_bar,
    Eigen::Matrix2f& bending_modulus,
    Eigen::MatrixXf& omega_bar_j_im1,
    Eigen::MatrixXf& omega_bar_j_i,
    const float beta,
    Eigen::VectorXf& initial_theta,
    char* boundry_conditions,
    std::vector<Eigen::Matrix3f>& bf,
    Eigen::MatrixXf& kb,
    const float tol,
    const int max_iters) {
    // Compute the initial omegas
    Eigen::MatrixXf omega_j_im1 = newtons_compute_omega(bf, initial_theta, kb, OMEGA_J_IM1);
    Eigen::MatrixXf omega_j_i = newtons_compute_omega(bf, initial_theta, kb, OMEGA_J_I);

    // Compute the initial gradient
    Eigen::VectorXf grad = compute_grad_dEdtheta(
        neighbor_len_bar,
        omega_j_im1,
        omega_j_i,
        bending_modulus,
        omega_bar_j_im1,
        omega_bar_j_i,
        beta,
        initial_theta,
        boundry_conditions);

    // Compute the initial hessian
    Eigen::SparseMatrix<float> hessian = compute_hessian_d2Edtheta2(
        beta,
        neighbor_len_bar,
        omega_j_im1,
        omega_j_i,
        bending_modulus,
        omega_bar_j_im1,
        omega_bar_j_i,
        boundry_conditions);

    int i = 0;
    Eigen::VectorXf theta = initial_theta;
    while (grad.norm() > tol && i < max_iters) {
        Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
        solver.compute(hessian);
        if (solver.info() != Eigen::Success) {
            std::cout << "Newtons: Decomposition failed!" << std::endl;
            return theta;
        }

        // Solve for the searh direction
        // This uses the 2nd order taylor approximation to the energy function
        Eigen::VectorXf d = solver.solve(-grad);
        if (solver.info() != Eigen::Success) {
            std::cout << "Newtons: Solving failed!" << std::endl;
            return theta;
        }

        // Update the theta parameter
        theta = theta + d;

        // Compute the omegas
        omega_j_im1 = newtons_compute_omega(bf, theta, kb, OMEGA_J_IM1);
        omega_j_i = newtons_compute_omega(bf, theta, kb, OMEGA_J_I);

        // Compute the gradient
        grad = compute_grad_dEdtheta(
            neighbor_len_bar,
            omega_j_im1,
            omega_j_i,
            bending_modulus,
            omega_bar_j_im1,
            omega_bar_j_i,
            beta,
            theta,
            boundry_conditions);

        // Compute the hessian
        hessian = compute_hessian_d2Edtheta2(
            beta,
            neighbor_len_bar,
            omega_j_im1,
            omega_j_i,
            bending_modulus,
            omega_bar_j_im1,
            omega_bar_j_i,
            boundry_conditions);

        i++;
    }

    return theta;
}