#pragma once

/**
 * @file include/myrtchallenge/tuples.hpp
 * @brief Declaration of tuple, point, vector and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>


struct Tuple
{
    double_t x{NAN};
    double_t y{NAN};
    double_t z{NAN};
    double_t w{NAN};

    // a tuple with w=1.0 is a point
    bool is_point() const { return this->w == 1.0; }

    // a tuple with w=0.0 is a vector
    bool is_vector() const { return this->w == 0.0; }

    bool operator==(const Tuple& rhs) const;
    bool operator!=(const Tuple& rhs) const;
};


Tuple point(double_t x, double_t y, double_t z);
Tuple tuple(double_t x, double_t y, double_t z, double_t w);
Tuple vector(double_t x, double_t y, double_t z);

Tuple operator-(const Tuple& lhs);

Tuple operator+(const Tuple &lhs, const Tuple &rhs);
Tuple operator-(const Tuple& lhs, const Tuple& rhs);

Tuple operator*(const Tuple& lhs, double_t rhs);
Tuple operator/(const Tuple& lhs, double_t rhs);

double_t dot(const Tuple& lhs, const Tuple& rhs);
Tuple cross(const Tuple& lhs, const Tuple& rhs);
Tuple normalize(const Tuple& tpl);
double_t magnitude(const Tuple& tpl);
Tuple reflect(const Tuple& in, const Tuple& normal);
