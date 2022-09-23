/**
 * @file src/tuples.cpp
 * @brief Definition of tuple, point, vector and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include "myrtchallenge/tuples.hpp"
#include "primitives.hpp"


bool Tuple::operator==(const Tuple& rhs) const
{
    return equal(x, rhs.x) && equal(y, rhs.y) && equal(z, rhs.z) && equal(w, rhs.w);
}


bool Tuple::operator!=(const Tuple& rhs) const
{
    return !(*this == rhs);
}


Tuple point(double_t x, double_t y, double_t z)
{
    return tuple(x, y, z, 1);
}


Tuple tuple(double_t x, double_t y, double_t z, double_t w)
{
    return Tuple{x, y, z, w};
}


Tuple vector(double_t x, double_t y, double_t z)
{
    return tuple(x, y, z, 0);
}


// negation
Tuple operator-(const Tuple& lhs)
{
    return tuple(-lhs.x, -lhs.y, -lhs.z, -lhs.w);
}


// addition
Tuple operator+(const Tuple& lhs, const Tuple& rhs)
{
    return tuple(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}


// subtraction
Tuple operator-(const Tuple& lhs, const Tuple& rhs)
{
    return tuple(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}


// scalar multiplication
Tuple operator*(const Tuple& lhs, double_t rhs)
{
    return tuple(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}


// scalar division
Tuple operator/(const Tuple& lhs, double_t rhs)
{
    return tuple(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}


Tuple cross(const Tuple& lhs, const Tuple& rhs)
{
    return tuple(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
        0
    );
}


double_t dot(const Tuple& lhs, const Tuple& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}


double_t magnitude(const Tuple& tpl)
{
    return std::sqrt(std::pow(tpl.x, 2) + std::pow(tpl.y, 2) + std::pow(tpl.z, 2) + std::pow(tpl.w, 2));
}


Tuple normalize(const Tuple& tpl)
{
    auto mag = magnitude(tpl);
    return tuple(tpl.x / mag, tpl.y / mag, tpl.z / mag, tpl.w / mag);
}

Tuple reflect(const Tuple& in, const Tuple& normal)
{
    return in - normal * 2 * dot(in, normal);
}
