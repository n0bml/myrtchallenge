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
    if (std::isinf(a) || std::isinf(b))
        return a == b;
    else if (std::abs(a - b) < EPSILON)
        return true;
    return false;
}


inline std::pair<double_t, double_t> check_axis(double_t origin, double_t direction, double_t min = -1, double_t max = 1)
{
    auto tmin_numerator = min - origin;
    auto tmax_numerator = max - origin;
    double_t tmin, tmax;
    if (std::fabs(direction) >= EPSILON) {
        tmin = tmin_numerator / direction;
        tmax = tmax_numerator / direction;
    }
    else {
        tmin = tmin_numerator * INFINITY;
        tmax = tmax_numerator * INFINITY;
    }
    if (tmin > tmax)
        std::swap(tmin, tmax);
    return std::make_pair(tmin, tmax);
}
