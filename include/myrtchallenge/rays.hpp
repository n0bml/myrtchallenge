#pragma once

/**
 * @file src/rays.cpp
 * @brief Declaration of rays and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#include "myrtchallenge/matrices.hpp"
#include "myrtchallenge/tuples.hpp"


struct Ray
{
    Tuple origin;
    Tuple direction;

    bool operator==(const Ray& rhs) const;
};


Ray ray(const Tuple& origin, const Tuple& direction);

Tuple position(const Ray& ray, double_t t);
Ray transform(const Ray& ray, const Matrix& m);
