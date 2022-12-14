#pragma once

#include <vector>

#include <Eigen/Core>

#include "compute_material_frame.h"
#include "compute_omega.h"
#include "compute_grad_dEdtheta.h"
#include "compute_hessian_d2Edtheta2.h"
#include "../../math/tridiagonal_solve.h"

Omega newtons_compute_omega(std::vector<Eigen::Matrix3f>& bf,
                            Eigen::VectorXf& theta,
                            Eigen::MatrixXf& kb) {
    // Compute Material Frame
    std::vector<Eigen::Matrix3f> mf = compute_material_frame(bf, theta);

    // Compute Omega
    Omega omega = compute_omega(kb, mf);
    return omega;
}

Eigen::VectorXf newtons_method_minimization(
    Eigen::VectorXf& neighbor_len_bar,
    Eigen::Matrix2f& bending_modulus,
    Omega& omega_bar,
    const float beta,
    Eigen::VectorXf& initial_theta,
    char* boundry_conditions,
    std::vector<Eigen::Matrix3f>& bf,
    Eigen::MatrixXf& kb,
    const float tol,
    const int max_iters) {
    int num_segments = neighbor_len_bar.size() + 1;
    // Compute the initial omegas
    Omega omega = newtons_compute_omega(bf, initial_theta, kb);
    Eigen::MatrixXf omega_j_im1 = omega.omega_j_im1;
    Eigen::MatrixXf omega_j_i = omega.omega_j_i;

    // Compute the initial gradient
    Eigen::VectorXf grad = compute_grad_dEdtheta(
        neighbor_len_bar,
        omega,
        bending_modulus,
        omega_bar,
        beta,
        initial_theta,
        boundry_conditions);
    grad *= -1.0;

    // Compute the initial hessian
    Hessian hessian = compute_hessian_d2Edtheta2(
        beta,
        neighbor_len_bar,
        omega,
        bending_modulus,
        omega_bar,
        boundry_conditions);

    int i = 0;
    Eigen::VectorXf theta = initial_theta;
    while (grad.norm() > tol && i < max_iters) {
        // Solve for the search direction
        // This uses the 2nd order taylor approximation to the energy function
        Eigen::VectorXf d = tridiagonal_solve(hessian.lower, hessian.center, hessian.upper, grad);

        // Update the theta parameter
        theta += d;

        // Compute the omegas
        omega = newtons_compute_omega(bf, theta, kb);

        // Compute the gradient
        grad = compute_grad_dEdtheta(
            neighbor_len_bar,
            omega,
            bending_modulus,
            omega_bar,
            beta,
            theta,
            boundry_conditions);
        grad *= -1.0f;

        // Compute the hessian
        hessian = compute_hessian_d2Edtheta2(
            beta,
            neighbor_len_bar,
            omega,
            bending_modulus,
            omega_bar,
            boundry_conditions);

        i++;
    }

    return theta;
}