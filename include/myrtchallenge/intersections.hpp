#pragma once

/**
 * @file include/myrtchallenge/intersections.cpp
 * @brief Declaration of intersections and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include <initializer_list>
#include <memory>
#include <vector>

#include "myrtchallenge/rays.hpp"


struct Shape;
using Shape_Ptr = std::shared_ptr<Shape>;


struct Intersection
{
    double_t t;
    Shape_Ptr object;

    bool operator<(const Intersection& rhs) const;
    bool operator==(const Intersection& rhs) const;

    bool hit() const { return !std::isnan(t); }
};


using Intersections = std::vector<Intersection>;

Intersection hit(const Intersections& is);
Intersection intersection(double_t t, Shape_Ptr object);
Intersections intersections(const Intersections& is);


struct Computations
{
    double_t t;
    Shape_Ptr object;
    Tuple point;
    Tuple eyev;
    Tuple normalv;
    bool inside;
    Tuple over_point;
    Tuple reflectv;
    double_t n1;
    double_t n2;
    Tuple under_point;
};


Computations prepare_computations(const Intersection& i, const Ray& ray, const Intersections& xs = std::vector<Intersection>{});
