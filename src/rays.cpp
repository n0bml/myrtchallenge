/**
 * @file src/rays.cpp
 * @brief Definition of rays and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#include "myrtchallenge/rays.hpp"
#include "myrtchallenge/tuples.hpp"

#include "primitives.hpp"


bool Ray::operator==(const Ray& rhs) const
{
    return origin == rhs.origin && direction == rhs.direction;
}


Ray ray(const Tuple& origin, const Tuple& direction)
{
    return Ray{origin, direction};
}


Tuple position(const Ray& ray, double_t t)
{
    return ray.origin + ray.direction * t;
}


Ray transform(const Ray& ray, const Matrix& m)
{
    return Ray{m * ray.origin, m * ray.direction};
}
