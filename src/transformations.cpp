/**
 * @file src/transformations.cpp
 * @brief Definitions for matrix transformations and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include "myrtchallenge/transformations.hpp"


Matrix rotation_x(double_t rad)
{
    auto rot = identity_matrix();
    rot(1, 1) =  std::cos(rad);
    rot(1, 2) = -std::sin(rad);
    rot(2, 1) =  std::sin(rad);
    rot(2, 2) =  std::cos(rad);
    return rot;
}


Matrix rotation_y(double_t rad)
{
    auto rot = identity_matrix();
    rot(0, 0) =  std::cos(rad);
    rot(0, 2) =  std::sin(rad);
    rot(2, 0) = -std::sin(rad);
    rot(2, 2) =  std::cos(rad);
    return rot;
}


Matrix rotation_z(double_t rad)
{
    auto rot = identity_matrix();
    rot(0, 0) =  std::cos(rad);
    rot(0, 1) = -std::sin(rad);
    rot(1, 0) =  std::sin(rad);
    rot(1, 1) =  std::cos(rad);
    return rot;
}


Matrix scaling(double_t x, double_t y, double_t z)
{
    auto s = identity_matrix();
    s(0, 0) = x;
    s(1, 1) = y;
    s(2, 2) = z;
    return s;
}


Matrix shearing(double_t xy, double_t xz, double_t yx, double_t yz, double_t zx, double_t zy)
{
    auto s = identity_matrix();
    s(0, 1) = xy;
    s(0, 2) = xz;
    s(1, 0) = yx;
    s(1, 2) = yz;
    s(2, 0) = zx;
    s(2, 1) = zy;
    return s;
}


Matrix translation(double_t x, double_t y, double_t z)
{
    auto t = identity_matrix();
    t(0, 3) = x;
    t(1, 3) = y;
    t(2, 3) = z;
    return t;
}


Matrix view_transform(const Tuple& from, const Tuple& to, const Tuple& up)
{
    auto forward = normalize(to - from);
    auto upn = normalize(up);
    auto left = cross(forward, upn);
    auto true_up = cross(left, forward);

    auto orientation = matrix(4, {
         left.x,     left.y,     left.z,    0,
         true_up.x,  true_up.y,  true_up.z, 0,
        -forward.x, -forward.y, -forward.z, 0,
         0,          0,          0,         1,
    });

    return orientation * translation(-from.x, -from.y, -from.z);
}
