/**
 * @file src/world.cpp
 * @brief Definition of rendering world and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <algorithm>
#include <cmath>

#include "myrtchallenge/shapes.hpp"
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


Color color_at(const World_Ptr& world, const Ray& ray, int remaining)
{
    auto xs = intersect_world(world, ray);
    auto x = hit(xs);
    if (!x.hit())
        return color(0, 0, 0);
    auto comps = prepare_computations(x, ray, xs);
    return shade_hit(world, comps, remaining);
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


bool is_shadowed(const World_Ptr& world, const Tuple& point)
{
    auto v = world->light->position - point;
    auto distance = magnitude(v);
    auto direction = normalize(v);

    auto r = ray(point, direction);
    auto xs = intersect_world(world, r);

    auto h = hit(xs);
    if (h.hit() && h.t < distance)
        return true;
    return false;
}


Color reflected_color(const World_Ptr& world, const Computations& comps, int remaining)
{
    if (remaining <= 0)
        return color(0, 0, 0);

    if (comps.object->material->reflective == 0)
        return color(0, 0, 0);

    auto reflect_ray = ray(comps.over_point, comps.reflectv);
    auto clr = color_at(world, reflect_ray, remaining - 1);
    return clr * comps.object->material->reflective;
}


Color refracted_color(const World_Ptr& world, const Computations& comps, int remaining)
{
    if (remaining <= 0)
        return color(0, 0, 0);

    if (comps.object->material->transparency == 0)
        return color(0, 0, 0);

    // Find the ratio of first index of refraction to the second.
    // (Yup, this is inverted from the definition of Snell's Law.)
    auto n_ratio = comps.n1 / comps.n2;

    // cos(theta_i) is the same as the dot product of the two vectors
    auto cos_i = dot(comps.eyev, comps.normalv);

    // Find sin(theta_t)^2 via trigonomic identity
    auto sin2_t = std::pow(n_ratio, 2) * (1 - std::pow(cos_i, 2));
    if (sin2_t > 1)
        return color(0, 0, 0);

    // Find cos(theta_t) via trigonomic identity
    auto cos_t = std::sqrt(1.0 - sin2_t);

    // Compute the direction of the refracted ray
    auto direction = comps.normalv * (n_ratio * cos_i - cos_t) - comps.eyev * n_ratio;

    // Create the refracted ray
    auto refract_ray = ray(comps.under_point, direction);

    // Find the color of the refracted ray, making sure to multiply
    // by the transparency value to account for any opacity
    return color_at(world, refract_ray, remaining - 1) * comps.object->material->transparency;
}


Color shade_hit(const World_Ptr& world, const Computations& comps, int remaining)
{
    auto shadowed = is_shadowed(world, comps.over_point);

    auto surface = lighting(comps.object->material, comps.object, world->light, comps.over_point, comps.eyev, comps.normalv, shadowed);

    auto reflected = reflected_color(world, comps, remaining);
    auto refracted = refracted_color(world, comps, remaining);

    auto material = comps.object->material;
    if (material->reflective > 0 && material->transparency > 0) {
        auto reflectance = schlick(comps);
        return surface + reflected * reflectance + refracted * (1 - reflectance);
    }
    else
        return surface + reflected + refracted;
}
