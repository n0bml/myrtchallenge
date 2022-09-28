/**
 * @file src/spheres.cpp
 * @brief Definition of spheres and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#include "myrtchallenge/spheres.hpp"

#include "primitives.hpp"

bool Sphere::operator==(const Sphere& rhs) const
{
    if (transform != rhs.transform) {
        return false;
    }
    if (*material != *rhs.material) {
        return false;
    }
    return true;
}


Sphere_Ptr sphere()
{
    return std::make_shared<Sphere>(identity_matrix(), material());
}


Intersections intersect(Sphere_Ptr sphere, const Ray& ray)
{
    Intersections results;

    auto ray2 = transform(ray, inverse(sphere->transform));

    // the vector from the sphere's center to the ray origin
    // remember: the sphere is centered at the world origin
    auto sphere_to_ray = ray2.origin - point(0, 0, 0);

    auto a = dot(ray2.direction, ray2.direction);
    auto b = 2 * dot(ray2.direction, sphere_to_ray);
    auto c = dot(sphere_to_ray, sphere_to_ray) - 1;

    auto discriminant = std::pow(b, 2) - 4 * a * c;
    if (discriminant < 0)
        return results;

    results.resize(2);
    results[0] = Intersection{(-b - std::sqrt(discriminant)) / (2 * a), sphere};
    results[1] = Intersection{(-b + std::sqrt(discriminant)) / (2 * a), sphere};

    return results;
}


Tuple normal_at(Sphere_Ptr sphere, const Tuple& world_point)
{
    auto object_point = inverse(sphere->transform) * world_point;
    auto object_normal = object_point - point(0, 0, 0);
    auto world_normal = transpose(inverse(sphere->transform)) * object_normal;
    world_normal.w = 0;
    return normalize(world_normal);
}


void set_transform(Sphere_Ptr sphere, const Matrix& m)
{
    sphere->transform = m;
}
