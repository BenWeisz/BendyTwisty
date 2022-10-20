#pragma once

#include <vector>

#include <Eigen/Core>
#include <Eigen/Sparse>

#include "util.h"

Eigen::SparseMatrix<float> compute_hessian_d2Edtheta2(
    float beta,
    Eigen::VectorXf& neighbor_len_bar,
    Omega& omega,
    Eigen::Matrix2f& bending_modulus,
    Omega& omega_bar,
    char* boundry_conditions) {
    int num_segments = neighbor_len_bar.size() + 1;

    std::vector<Eigen::Triplet<float>> coeffs;

    // J matrix rotates PI/2 CC
    Eigen::Matrix2f J;
    J << 0, -1, 1, 0;

    int free_count = 0;
    for (int i = 0; i < num_segments; i++)
        if (boundry_conditions[i] == VERTEX_STRESS_FREE)
            free_count++;

    int k = 0;
    // Fill the hessian with values
    for (int i = 0; i < num_segments; i++) {
        if (boundry_conditions[i] == VERTEX_CLAMPED)
            continue;

        // j, j-1 term
        if (i - 1 >= 0) {
            float beta_term = 2 * beta / neighbor_len_bar(i - 1);
            if (k - 1 >= 0) {
                Eigen::Triplet<float> v(k, k - 1, -beta_term);
                coeffs.push_back(v);
            }

            // subterms for j,j where j >= 1
            Eigen::Vector2f wij = omega.omega_j_i.col(i - 1);
            Eigen::Vector2f wij_bar = omega_bar.omega_j_i.col(i - 1);

            // term 1: 1/lbar_i * (w^j_i)^T * J^T * Bbar * J * w^j_i
            float term1 = (wij.dot(J.transpose() * bending_modulus * J * wij)) / neighbor_len_bar(i - 1);

            // term 2: 1/lbar_i * (w^j_i)^T * Bbar * (w^j_i - wbar^j_i)
            float term2 = (wij.dot(bending_modulus * (wij - wij_bar))) / neighbor_len_bar(i - 1);

            float jj_term = beta_term + term1 - term2;
            Eigen::Triplet<float> jj(k, k, jj_term);
            coeffs.push_back(jj);
        }

        // j, j+1 term
        if (i + 1 < num_segments) {
            float beta_term = 2 * beta / neighbor_len_bar(i);
            if (k + 1 < free_count) {
                Eigen::Triplet<float> v(k, k + 1, -beta_term);
                coeffs.push_back(v);
            }

            // subterms for j,j where 0 <= j <= num_segments - 2
            Eigen::Vector2f wij = omega.omega_j_im1.col(i);
            Eigen::Vector2f wij_bar = omega_bar.omega_j_im1.col(i);

            // term 1: 1/lbar_i * (w^j_i)^T * J^T * Bbar * J * w^j_i
            float term1 = (wij.dot(J.transpose() * bending_modulus * J * wij)) / neighbor_len_bar(i);

            // term 2: 1/lbar_i * (w^j_i)^T * Bbar * (w^j_i - wbar^j_i)
            float term2 = (wij.dot(bending_modulus * (wij - wij_bar))) / neighbor_len_bar(i);

            float jj_term = beta_term + term1 - term2;
            Eigen::Triplet<float> jj(k, k, jj_term);
            coeffs.push_back(jj);
        }

        // This is only incremented if the vertex at i was not clamped
        k++;
    }

    // The hessian matrix
    Eigen::SparseMatrix<float> hessian(free_count, free_count);
    hessian.setZero();
    hessian.setFromTriplets(coeffs.begin(), coeffs.end());
    return hessian;
}