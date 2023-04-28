/**
 * @file src/shapes.cpp
 * @brief Definition of shape base class and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <algorithm>
#include <cmath>

#include "myrtchallenge/shapes.hpp"

#include "primitives.hpp"


std::pair<double_t, double_t> check_axis(double_t origin, double_t direction)
{
    auto tmin_numerator = (-1 - origin);
    auto tmax_numerator = ( 1 - origin);

    double_t tmin, tmax;
    if (std::fabs(direction) >= EPSILON) {
        tmin = tmin_numerator / direction;
        tmax = tmax_numerator / direction;
    } else {
        tmin = tmin_numerator * INFINITY;
        tmax = tmax_numerator * INFINITY;
    }

    if (tmin > tmax)
        std::swap(tmin, tmax);

    return std::make_pair(tmin, tmax);
}


bool Shape::operator==(const Shape& rhs) const
{
    if (transform != rhs.transform) {
        return false;
    }
    if (*material != *rhs.material) {
        return false;
    }
    return true;
}


/**
 * @brief Return the intersections between the given ray and the shape.
 *
 * @param shape
 * @param ray
 * @return Intersections
 */
Intersections intersect(Shape_Ptr shape, const Ray& ray)
{
    Intersections results;

    auto local_ray = transform(ray, inverse(shape->transform));
    return shape->local_intersect(local_ray);
}


/**
 * @brief Return the normal at the given point on the shape.
 *
 * @param shape
 * @param point
 * @return Tuple
 */
Tuple normal_at(Shape_Ptr shape, const Tuple& point)
{
    auto local_point = inverse(shape->transform) * point;
    auto local_normal = shape->local_normal_at(local_point);
    auto world_normal = transpose(inverse(shape->transform)) * local_normal;
    world_normal.w = 0;
    return normalize(world_normal);
}


/**
 * @brief Set the transformation matrix for the given shape.
 *
 * @param shape
 * @param m
 */
void set_transform(Shape_Ptr shape, const Matrix& m)
{
    shape->transform = m;
}


/**
 * @brief Construct and return a shared pointer to a Cube.
 *
 * @return Shape_Ptr
 */
Shape_Ptr cube()
{
    auto ptr = std::make_shared<Cube>();
    ptr->transform = identity_matrix();
    ptr->material = material();
    return ptr;
}


/**
 * @brief Return the intersections between the ray and the Cube.
 *
 * @param ray
 * @return Intersections
 */
Intersections Cube::local_intersect(const Ray& ray)
{
    auto [xtmin, xtmax] = check_axis(ray.origin.x, ray.direction.x);
    auto [ytmin, ytmax] = check_axis(ray.origin.y, ray.direction.y);
    auto [ztmin, ztmax] = check_axis(ray.origin.z, ray.direction.z);

    auto tmin = std::max({xtmin, ytmin, ztmin});
    auto tmax = std::min({xtmax, ytmax, ztmax});

    Intersections results;
    if (tmin <= tmax) {
        results.emplace_back(intersection(tmin, shared_from_this()));
        results.emplace_back(intersection(tmax, shared_from_this()));
    }
    return results;
}


/**
 * @brief Return the local normal at the given point.
 *
 * @param pt
 * @return Tuple
 */
Tuple Cube::local_normal_at(const Tuple& pt) const
{
    auto maxc = std::max({std::fabs(pt.x), std::fabs(pt.y), std::fabs(pt.z)});
    if (maxc == std::fabs(pt.x)) {
        return vector(pt.x, 0, 0);
    } else if (maxc == std::fabs(pt.y)) {
        return vector(0, pt.y, 0);
    } else {
        return vector(0, 0, pt.z);
    }
}


/**
 * @brief Construct and return a shared pointer to a Plane.
 *
 * @return Shape_Ptr
 */
Shape_Ptr plane()
{
    auto ptr = std::make_shared<Plane>();
    ptr->transform = identity_matrix();
    ptr->material = material();
    return ptr;
}


/**
 * @brief Return the intersections between the ray and the Plane.
 *
 * @param ray
 * @return Intersections
 */
Intersections Plane::local_intersect(const Ray& ray)
{
    Intersections results;
    if (std::fabs(ray.direction.y) < EPSILON)
        return results;
    auto t = -ray.origin.y / ray.direction.y;
    results.emplace_back(intersection(t, shared_from_this()));
    return results;
}


/**
 * @brief Return the local normal at the given point.
 *
 * @param pt - unused
 * @return Tuple
 */
Tuple Plane::local_normal_at(const Tuple& /*pt*/) const
{
    return vector(0, 1, 0);
}


/**
 * @brief Construct and return a shared pointer to a Sphere.
 *
 * @return Shape_Ptr
 */
Shape_Ptr sphere()
{
    auto ptr = std::make_shared<Sphere>();
    ptr->transform = identity_matrix();
    ptr->material = material();
    return ptr;
}


/**
 * @brief A helper for producing a sphere with a glassy material.
 *
 * @return Shape_Ptr
 */
Shape_Ptr glass_sphere()
{
    auto s = sphere();
    s->material->transparency = 1.0;
    s->material->refractive_index = 1.5;
    return s;
}

/**
 * @brief Return the intersections between the ray and the Sphere.
 *
 * @param ray
 * @return Intersections
 */
Intersections Sphere::local_intersect(const Ray& ray)
{
    Intersections results;

    // the vector from the sphere's center to the ray origin
    // remember: the sphere is centered at the world origin
    auto sphere_to_ray = ray.origin - point(0, 0, 0);

    auto a = dot(ray.direction, ray.direction);
    auto b = 2 * dot(ray.direction, sphere_to_ray);
    auto c = dot(sphere_to_ray, sphere_to_ray) - 1;

    auto discriminant = std::pow(b, 2) - 4 * a * c;
    if (discriminant < 0)
        return results;

    results.reserve(2);
    results.emplace_back(intersection((-b - std::sqrt(discriminant)) / (2 * a), shared_from_this()));
    results.emplace_back(intersection((-b + std::sqrt(discriminant)) / (2 * a), shared_from_this()));

    return results;
}


/**
 * @brief Return the local normal at the given point.
 *
 * @param pt
 * @return Tuple
 */
Tuple Sphere::local_normal_at(const Tuple& pt) const
{
    return pt - point(0, 0, 0);
}
