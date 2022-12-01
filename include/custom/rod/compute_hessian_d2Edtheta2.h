#pragma once

#include <vector>

#include <Eigen/Core>

#include "util.h"

typedef struct Hessian {
    Eigen::VectorXf lower;
    Eigen::VectorXf center;
    Eigen::VectorXf upper;
} Hessian;

Hessian compute_hessian_d2Edtheta2(
    float beta,
    Eigen::VectorXf& neighbor_len_bar,
    Omega& omega,
    Eigen::Matrix2f& bending_modulus,
    Omega& omega_bar,
    char* boundry_conditions) {
    int num_segments = neighbor_len_bar.size() + 1;

    Eigen::VectorXf upper = Eigen::VectorXf::Zero(num_segments);
    Eigen::VectorXf center = Eigen::VectorXf::Zero(num_segments);
    Eigen::VectorXf lower = Eigen::VectorXf::Zero(num_segments);

    // J matrix rotates PI/2 CC
    Eigen::Matrix2f J;
    J << 0, -1, 1, 0;

    // Fill the hessian with values
    for (int i = 0; i < num_segments; i++) {
        // We don't want the clamped angles to be updated so we just set these entries to a 1x1 identity
        if (boundry_conditions[i] == EDGE_CLAMPED) {
            center(i) = 1.0f;
            if (i > 0)
                lower(i - 1) = 0.0f;
            if (i < num_segments - 1)
                upper(i + 1) = 0.0f;
            continue;
        }

        // j, j-1 term
        if (i - 1 >= 0) {
            float beta_term = 2 * beta / neighbor_len_bar(i - 1);
            lower(i) += -beta_term;

            // subterms for j,j where j >= 1
            Eigen::Vector2f wij = omega.omega_j_i.col(i - 1);
            Eigen::Vector2f wij_bar = omega_bar.omega_j_i.col(i - 1);

            // term 1: 1/lbar_i * (w^j_i)^T * J^T * Bbar * J * w^j_i
            float term1 = (wij.dot(J.transpose() * bending_modulus * J * wij)) / neighbor_len_bar(i - 1);

            // term 2: 1/lbar_i * (w^j_i)^T * Bbar * (w^j_i - wbar^j_i)
            float term2 = (wij.dot(bending_modulus * (wij - wij_bar))) / neighbor_len_bar(i - 1);

            float jj_term = beta_term + term1 - term2;
            center(i) += jj_term;
        }

        // j, j+1 term
        if (i + 1 < num_segments) {
            float beta_term = 2 * beta / neighbor_len_bar(i);
            upper(i) += -beta_term;

            // subterms for j,j where 0 <= j <= num_segments - 2
            Eigen::Vector2f wij = omega.omega_j_im1.col(i);
            Eigen::Vector2f wij_bar = omega_bar.omega_j_im1.col(i);

            // term 1: 1/lbar_i * (w^j_i)^T * J^T * Bbar * J * w^j_i
            float term1 = (wij.dot(J.transpose() * bending_modulus * J * wij)) / neighbor_len_bar(i);

            // term 2: 1/lbar_i * (w^j_i)^T * Bbar * (w^j_i - wbar^j_i)
            float term2 = (wij.dot(bending_modulus * (wij - wij_bar))) / neighbor_len_bar(i);

            float jj_term = beta_term + term1 - term2;
            center(i) += jj_term;
        }
    }

    Hessian hessian;
    hessian.lower = lower;
    hessian.center = center;
    hessian.upper = upper;

    return hessian;
}