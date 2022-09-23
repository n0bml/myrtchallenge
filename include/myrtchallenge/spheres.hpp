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
#include "myrtchallenge/materials.hpp"
#include "myrtchallenge/matrices.hpp"
#include "myrtchallenge/rays.hpp"


struct Sphere
{
    Matrix transform;
    Material_Ptr material;
};


using Sphere_Ptr = std::shared_ptr<Sphere>;


Sphere_Ptr sphere();


Intersections intersect(Sphere_Ptr sphere, const Ray& ray);
Tuple normal_at(Sphere_Ptr sphere, const Tuple& world_point);
void set_transform(Sphere_Ptr sphere, const Matrix& m);
