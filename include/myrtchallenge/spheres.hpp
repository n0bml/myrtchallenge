#pragma once

/**
 * @file src/spheres.cpp
 * @brief Declaration of spheres and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include <memory>
#include <vector>

#include "myrtchallenge/intersections.hpp"
#include "myrtchallenge/matrices.hpp"
#include "myrtchallenge/rays.hpp"


struct Sphere
{
    Matrix transform;
};


using SpherePtr = std::shared_ptr<Sphere>;


SpherePtr sphere();


Intersections intersect(SpherePtr sphere, const Ray& ray);
void set_transform(SpherePtr sphere, const Matrix& m);
