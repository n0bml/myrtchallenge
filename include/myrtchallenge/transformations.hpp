#pragma once

/**
 * @file include/myrtchallenge/transformations.cpp
 * @brief Declarations for matrix transformations and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#include "myrtchallenge/matrices.hpp"
#include "myrtchallenge/tuples.hpp"


Matrix rotation_x(double_t rad);
Matrix rotation_y(double_t rad);
Matrix rotation_z(double_t rad);
Matrix scaling(double_t x, double_t y, double_t z);
Matrix shearing(double_t xy, double_t xz, double_t yx, double_t yz, double_t zx, double_t zy);
Matrix translation(double_t x, double_t y, double_t z);
Matrix view_transform(const Tuple& from, const Tuple& to, const Tuple& up);
