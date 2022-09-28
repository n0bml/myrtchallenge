/**
 * @file src/world.cpp
 * @brief Definition of rendering world and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <algorithm>
#include <cmath>

#include "myrtchallenge/transformations.hpp"
#include "myrtchallenge/world.hpp"

#include "primitives.hpp"


World_Ptr world()
{
    return std::make_unique<World>();
}


World_Ptr default_world()
{
    auto w = world();
    w->light = point_light(point(-10, 10, -10), color(1, 1, 1));

    auto s1 = sphere();
    s1->material->color    = color(0.8, 1.0, 0.6);
    s1->material->diffuse  = 0.7;
    s1->material->specular = 0.2;
    w->objects.push_back(s1);

    auto s2 = sphere();
    s2->transform = scaling(0.5, 0.5, 0.5);
    w->objects.push_back(s2);

    return w;
}


Color color_at(const World_Ptr& world, const Ray& ray)
{
    auto xs = intersect_world(world, ray);
    auto x = hit(xs);
    if (!x.hit())
        return color(0, 0, 0);
    auto comps = prepare_computations(x, ray);
    return shade_hit(world, comps);
}


Intersections intersect_world(const World_Ptr& world, const Ray& ray)
{
    Intersections results;
    for (auto& o : world->objects) {
        auto txs = intersect(o, ray);
        results.insert(std::end(results), std::begin(txs), std::end(txs));
    }
    std::sort(std::begin(results), std::end(results));
    return results;
}


Color shade_hit(const World_Ptr& world, const Computations& comps)
{
    return lighting(comps.object->material, world->light, comps.point, comps.eyev, comps.normalv);
}
