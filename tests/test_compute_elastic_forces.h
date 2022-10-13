#pragma once

#include <Eigen/Core>

#include "../include/custom/rod/compute_elastic_forces.h"
#include "../include/custom/rod/compute_pdE_pdx.h"
#include "../include/custom/rod/compute_grad_holonomy.h"
#include "../include/custom/rod/util.h"
#include "util.h"

TEST_CASE("Test coil", "[test_compute_elastic_forces]") {
    int num_segments = 4;

    EPGradX pdE_pdx;

    Eigen::Vector3f dEdx0;
    dEdx0 << -4, 1, 6;
    Eigen::Vector3f dEdx1;
    dEdx1 << 4, 1, -12;
    Eigen::Vector3f dEdx2;
    dEdx2 << 8, 0, 8;
    Eigen::Vector3f dEdx3;
    dEdx3 << -13, -1, 3;
    Eigen::Vector3f dEdx4;
    dEdx4 << 5, -1, -5;
    pdE_pdx.push_back(dEdx0);
    pdE_pdx.push_back(dEdx1);
    pdE_pdx.push_back(dEdx2);
    pdE_pdx.push_back(dEdx3);
    pdE_pdx.push_back(dEdx4);

    Eigen::VectorXf pdE_pdtheta(4);
    pdE_pdtheta << 0, 0, 2, 6;

    PsiGradSum psi_grad_sum;
    Eigen::MatrixXf psi_grad0(3, num_segments - 1);
    psi_grad0 << 0, 0, 0, 0, 0, 0, -1, -1, -1;
    Eigen::MatrixXf psi_grad1(3, num_segments - 1);
    psi_grad1 << 0, 0, 0, 0, 0, 0, 0, -1, -1;
    Eigen::MatrixXf psi_grad2(3, num_segments - 1);
    psi_grad2 << 0, 0, 0, 0, 0, 0, 1, 1, 0;
    Eigen::MatrixXf psi_grad3(3, num_segments - 1);
    psi_grad3 << 0, 0, 0, 0, 0, 0, 0, 1, 1;
    Eigen::MatrixXf psi_grad4(3, num_segments - 1);
    psi_grad4 << 0, 0, 0, 0, 0, 0, 0, 0, 1;
    psi_grad_sum.push_back(psi_grad0);
    psi_grad_sum.push_back(psi_grad1);
    psi_grad_sum.push_back(psi_grad2);
    psi_grad_sum.push_back(psi_grad3);
    psi_grad_sum.push_back(psi_grad4);

    char* boundary_conditions = new char[num_segments];
    for (int i = 0; i < num_segments; i++)
        boundary_conditions[i] = VERTEX_STRESS_FREE;

    boundary_conditions[0] = VERTEX_CLAMPED;
    boundary_conditions[num_segments - 1] = VERTEX_CLAMPED;

    FORCE elastic_force = compute_elastic_forces(pdE_pdx, pdE_pdtheta, psi_grad_sum, boundary_conditions);

    Eigen::Vector3f F0;
    F0 << 4, -1, -12;
    Eigen::Vector3f F1;
    F1 << -4, -1, 6;
    Eigen::Vector3f F2;
    F2 << -8, 0, -8;
    Eigen::Vector3f F3;
    F3 << 13, 1, 3;
    Eigen::Vector3f F4;
    F4 << -5, 1, 11;

    REQUIRE(matrices_are_equal(elastic_force.segment(0, 3), F0, 3));
    REQUIRE(matrices_are_equal(elastic_force.segment(1 * 3, 3), F1, 3));
    REQUIRE(matrices_are_equal(elastic_force.segment(2 * 3, 3), F2, 3));
    REQUIRE(matrices_are_equal(elastic_force.segment(3 * 3, 3), F3, 3));
    REQUIRE(matrices_are_equal(elastic_force.segment(4 * 3, 3), F4, 3));

    delete[] boundary_conditions;
}