#pragma once

/**
 * @file include/myrtchallenge/world.hpp
 * @brief Declaration of rendering world and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include <memory>

#include "myrtchallenge/lights.hpp"
#include "myrtchallenge/matrices.hpp"
#include "myrtchallenge/tuples.hpp"
#include "myrtchallenge/spheres.hpp"


struct World
{
    Point_Light_Ptr light;
    std::vector<Sphere_Ptr> objects;
};


using World_Ptr = std::unique_ptr<World>;


World_Ptr world();

World_Ptr default_world();

Color color_at(const World_Ptr& world, const Ray& ray);
Intersections intersect_world(const World_Ptr& world, const Ray& ray);
bool is_shadowed(const World_Ptr& world, const Tuple& point);
Color shade_hit(const World_Ptr& world, const Computations& comps);
