#include <catch2/catch_test_macros.hpp>

#include "tests/test_compute_edges.h"
#include "tests/test_compute_kb.h"
#include "tests/test_compute_phi.h"
#include "tests/test_parallel_transport_space.h"
#include "tests/test_parallel_transport_time.h"
#include "tests/test_compute_material_frame.h"
#include "tests/test_compute_omega.h"
#include "tests/test_compute_grad_dEdtheta.h"
#include "tests/test_compute_neighbor_len.h"
#include "tests/test_compute_hessian_d2Edtheta2.h"
#include "tests/test_compute_grad_holonomy.h"
#include "tests/test_compute_skew_matrices.h"
#include "tests/test_compute_grad_kb.h"
#include "tests/test_compute_grad_omega.h"
#include "tests/test_compute_pdE_pdx.h"
#include "tests/test_compute_pdE_pdtheta.h"
#include "tests/test_compute_elastic_forces.h"
#include "tests/test_compute_strain.h"
#include "tests/test_compute_grad_strain.h"
#include "tests/test_compute_first_u0.h"
#include "tests/test_compute_updated_u0.h"

#include "tests/test_tridiagonal_solve.h"