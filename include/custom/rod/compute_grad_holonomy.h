#pragma once

#include <Eigen/Core>

typedef struct PsiGrad {
    Eigen::MatrixXf psi_grad_im1;
    Eigen::MatrixXf psi_grad_i;
    Eigen::MatrixXf psi_grad_ip1;
} PsiGrad;

typedef struct PsiGradSum {
    Eigen::MatrixXf psi_grad_im1_sum;
    Eigen::MatrixXf psi_grad_i_sum;
    Eigen::MatrixXf psi_grad_ip1_sum;
} PsiGradSum;

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

    Eigen::MatrixXf psi_grad_im1_sum(3, num_segments - 1);
    Eigen::MatrixXf psi_grad_i_sum(3, num_segments - 1);
    Eigen::MatrixXf psi_grad_ip1_sum(3, num_segments - 1);

    psi_grad_im1_sum.col(0) = psi_grad.psi_grad_im1.col(0);
    psi_grad_i_sum.col(0) = psi_grad.psi_grad_i.col(0);
    psi_grad_ip1_sum.col(0) = psi_grad.psi_grad_ip1.col(0);

    for (int i = 1; i < num_segments - 1; i++) {
        psi_grad_im1_sum.col(i) = psi_grad_im1_sum.col(i - 1) + psi_grad.psi_grad_im1.col(i);
        psi_grad_i_sum.col(i) = psi_grad_i_sum.col(i - 1) + psi_grad.psi_grad_i.col(i);
        psi_grad_ip1_sum.col(i) = psi_grad_ip1_sum.col(i - 1) + psi_grad.psi_grad_ip1.col(i);
    }

    PsiGradSum psi_grad_sum = {
        psi_grad_im1_sum,
        psi_grad_i_sum,
        psi_grad_ip1_sum};

    return psi_grad_sum;
}