#pragma once

/**
 * @file src/primitives.hpp
 * @brief Primitive internal definitions and functions for myrtchallenge.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */


#include <tuple>
#include <cmath>


constexpr double_t EPSILON = 0.00001;


inline bool equal(double_t a, double_t b)
{
    if (std::abs(a - b) < EPSILON)
        return true;
    return false;
}
