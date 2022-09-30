#pragma once

#include <vector>

#include <Eigen/Core>

typedef struct PsiGrad {
    Eigen::MatrixXf psi_grad_im1;
    Eigen::MatrixXf psi_grad_i;
    Eigen::MatrixXf psi_grad_ip1;
} PsiGrad;

typedef std::vector<Eigen::MatrixXf> PsiGradSum;

// Compute the gradiend of the angle from the holonmy psi
PsiGrad compute_grad_holonomy(Eigen::MatrixXf& kb, Eigen::MatrixXf& ebar) {
    int num_segments = ebar.cols();

    Eigen::MatrixXf psi_grad_im1(3, num_segments - 1);
    Eigen::MatrixXf psi_grad_i(3, num_segments - 1);
    Eigen::MatrixXf psi_grad_ip1(3, num_segments - 1);

    Eigen::VectorXf ebar_mag2 = (ebar.transpose() * ebar).diagonal().cwiseSqrt() * 2.0;

    psi_grad_im1 = kb.array().rowwise() / ebar_mag2.segment(0, num_segments - 1).transpose().array();
    psi_grad_ip1 = (-kb).array().rowwise() / ebar_mag2.segment(1, num_segments - 1).transpose().array();
    psi_grad_i = -(psi_grad_im1 + psi_grad_ip1);

    PsiGrad psi_grad = {
        psi_grad_im1,
        psi_grad_i,
        psi_grad_ip1};

    return psi_grad;
}

PsiGradSum compute_grad_holonomy_sum(PsiGrad& psi_grad) {
    int num_segments = psi_grad.psi_grad_im1.cols() + 1;

    PsiGradSum psi_grad_sum;
    // Set up pass
    for (int i = 0; i < num_segments + 1; i++) {
        Eigen::MatrixXf grad_psi_sum_i = Eigen::MatrixXf::Zero(3, num_segments - 1);
        psi_grad_sum.push_back(grad_psi_sum_i);
    }

    // Population pass
    for (int j = 0; j < num_segments - 1; j++) {
        psi_grad_sum[j].col(j) = psi_grad.psi_grad_im1.col(j);
        psi_grad_sum[j + 1].col(j) = psi_grad.psi_grad_i.col(j);
        psi_grad_sum[j + 2].col(j) = psi_grad.psi_grad_ip1.col(j);
    }

    // Aggregation Pass
    for (int i = 0; i < num_segments + 1; i++) {
        for (int j = 1; j < num_segments - 1; j++) {
            psi_grad_sum[i].col(j) += psi_grad_sum[i].col(j - 1);
        }
    }

    return psi_grad_sum;
}