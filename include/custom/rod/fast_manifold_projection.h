#pragma once

#include <iostream>

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>

#include "compute_edges.h"
#include "compute_strain.h"
#include "compute_grad_strain.h"

bool entries_greater_than(Eigen::VectorXf v, const float threshold) {
    for (int i = 0; i < v.size(); i++) {
        if (v(i) <= threshold)
            return false;
    }
    return true;
}

// Perform fast manifold projection onto the constraint manifold.
// This is introduced in the [Goldenthal, 2007] paper
void fast_manifold_projection(Eigen::MatrixXf& x, Eigen::MatrixXf& xdot, Eigen::MatrixXf& ebar, const float dt, const float mass, const int max_iters) {
    int num_segments = x.cols() - 1;

    const float threshold = 1e-8;

    // Fast Manifold Projection requires that we us the pre-time integration x in the computation of then new xdot
    // At this point, when we call this function, our x has already been updated with xdot * dt so in this step we reverse that
    Eigen::MatrixXf xtilde = x - (dt * xdot);
    Eigen::MatrixXf e = compute_edges(x);
    Eigen::VectorXf strain = compute_strain(e, ebar);

    int i = 0;
    while (entries_greater_than(strain, threshold) && i < max_iters) {
        Eigen::SparseMatrix<float> grad_strain = compute_grad_strain(e);
        Eigen::SparseMatrix<float> A = grad_strain * grad_strain.transpose();
        A *= (dt * dt) / mass;

        Eigen::SimplicialLDLT<Eigen::SparseMatrix<float>> solver;
        solver.compute(A);
        if (solver.info() != Eigen::Success) {
            std::cout << "FMP: Projection aux var failure!" << std::endl;
            return;
        }

        Eigen::VectorXf delta_lambda = solver.solve(strain);
        if (solver.info() != Eigen::Success) {
            std::cout << "FMP: Projection aux var solve error!" << std::endl;
            return;
        }

        Eigen::VectorXf delta_x = (grad_strain.transpose() * delta_lambda) * ((-dt * dt) / mass);

        Eigen::MatrixXf delta_x_mat(3, num_segments + 1);
        for (int i = 0; i < num_segments + 1; i++) {
            delta_x_mat.col(i) = delta_x.segment(i * 3, 3);
        }

        // Update the last x
        x += delta_x_mat;

        e = compute_edges(x);
        strain = compute_strain(e, ebar);
        i++;
    }

    // Update the candidate velocity
    xdot = (x - xtilde) / dt;
}