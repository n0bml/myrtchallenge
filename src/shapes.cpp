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


bool check_cap(const Ray& ray, double_t t)
{
    auto x = ray.origin.x + t * ray.direction.x;
    auto z = ray.origin.z + t * ray.direction.z;
    return (std::pow(x, 2) + std::pow(z, 2)) <= 1;
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
 * @brief Construct and return a shared pointer to a Cone.
 *
 * @return Shape_Ptr
 */
Cone_Ptr cone()
{
    auto ptr = std::make_shared<Cone>();
    ptr->transform = identity_matrix();
    ptr->material = material();
    return ptr;
}


/**
 * @brief Return the intersections between the ray and the Cone.
 *
 * @param ray
 * @return Intersections
 */
Intersections Cone::local_intersect(const Ray& ray)
{
    Intersections results;
    intersect_caps(ray, results);

    auto a = std::pow(ray.direction.x, 2) - std::pow(ray.direction.y, 2) + std::pow(ray.direction.z, 2);
    auto b = 2 * ray.origin.x * ray.direction.x - 2 * ray.origin.y * ray.direction.y + 2 * ray.origin.z * ray.direction.z;
    auto c = std::pow(ray.origin.x, 2) - std::pow(ray.origin.y, 2) + std::pow(ray.origin.z, 2);

    if (equal(a, 0)) {
        if (!equal(b, 0)) {
            auto t = -c / (2 * b);
            results.emplace_back(Intersection{t, shared_from_this()});
        }
        return results;
    }

    auto disc = std::pow(b, 2) - 4 * a * c;

    // ray does not intersect cone
    if (disc < 0)
        return results;

    auto t0 = (-b - std::sqrt(disc)) / (2 * a);
    auto t1 = (-b + std::sqrt(disc)) / (2 * a);
    if (t0 > t1)
        std::swap(t0, t1);

    auto y0 = ray.origin.y + t0 * ray.direction.y;
    if (minimum < y0 && y0 < maximum)
        results.emplace_back(Intersection{t0, shared_from_this()});

    auto y1 = ray.origin.y + t1 * ray.direction.y;
    if (minimum < y1 && y1 < maximum)
        results.emplace_back(Intersection{t1, shared_from_this()});

    return results;
}


/**
 * @brief Return the local normal at the given point.
 *
 * @param pt
 * @return Tuple
 */
Tuple Cone::local_normal_at(const Tuple& pt) const
{
    // compute the square of the distance from the y axis
    auto dist = std::pow(pt.x, 2) + std::pow(pt.z, 2);
    if (dist < 1 && pt.y >= (maximum - EPSILON))
        return vector(0, 1, 0);
    else if (dist < 1 && pt.y <= (minimum + EPSILON))
        return vector(0, -1, 0);

    auto y = std::sqrt(std::pow(pt.x, 2) + std::pow(pt.z, 2));
    if (pt.y > 0)
        y = -y;
    return vector(pt.x, y, pt.z);
}



/**
 * @brief Construct and return a shared pointer to a Cube.
 *
 * @return Shape_Ptr
 */
Cube_Ptr cube()
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
 * @brief Construct and return a shared pointer to a Cylinder.
 *
 * @return Shape_Ptr
 */
Cylinder_Ptr cylinder()
{
    auto ptr = std::make_shared<Cylinder>();
    ptr->transform = identity_matrix();
    ptr->material = material();
    return ptr;
}


/**
 * @brief Return the intersections between the ray and the Cylinder.
 *
 * @param ray
 * @return Intersections
 */
Intersections Cylinder::local_intersect(const Ray& ray)
{
    Intersections results;
    intersect_caps(ray, results);

    auto a = std::pow(ray.direction.x, 2) + std::pow(ray.direction.z, 2);

    // ray is parallel to the y axis
    if (equal(a, 0))
        return results;

    auto b = 2 * ray.origin.x * ray.direction.x + 2 * ray.origin.z * ray.direction.z;
    auto c = std::pow(ray.origin.x, 2) + std::pow(ray.origin.z, 2) - 1;
    auto disc = std::pow(b, 2) - 4 * a * c;

    // ray does not intersesect cylinder
    if (disc < 0)
        return results;

    auto t0 = (-b - std::sqrt(disc)) / (2 * a);
    auto t1 = (-b + std::sqrt(disc)) / (2 * a);
    if (t0 > t1)
        std::swap(t0, t1);

    auto y0 = ray.origin.y + t0 * ray.direction.y;
    if (minimum < y0 && y0 < maximum)
        results.emplace_back(intersection(t0, shared_from_this()));

    auto y1 = ray.origin.y + t1 * ray.direction.y;
    if (minimum < y1 && y1 < maximum)
        results.emplace_back(intersection(t1, shared_from_this()));

    return results;
}


/**
 * @brief Return the local normal at the given point.
 *
 * @param pt
 * @return Tuple
 */
Tuple Cylinder::local_normal_at(const Tuple& pt) const
{
    // compute the square of the distance from the y axis
    auto dist = std::pow(pt.x, 2) + std::pow(pt.z, 2);
    if (dist < 1 && pt.y >= (maximum - EPSILON))
        return vector(0, 1, 0);
    else if (dist < 1 && pt.y <= (minimum + EPSILON))
        return vector(0, -1, 0);
    else
        return vector(pt.x, 0, pt.z);
}


void Cylinder::intersect_caps(const Ray& ray, Intersections& xs)
{
    // caps only matter if the cylinder is closed, and might possibly be
    // intersected by the ray
    if (!closed || equal(ray.direction.y, 0))
        return;

    // check for an intersection with the lower end cap by intersecting
    // the ray with the plane at y=cyl.minimum
    auto t = (minimum - ray.origin.y) / ray.direction.y;
    if (check_cap(ray, t))
        xs.emplace_back(Intersection{t, shared_from_this()});

    // check for an intersection with the upper end cap by intersecting
    // the ray with the plane at y=cyl.maximum
    t = (maximum - ray.origin.y) / ray.direction.y;
    if (check_cap(ray, t))
        xs.emplace_back(Intersection{t, shared_from_this()});
}


/**
 * @brief Construct and return a shared pointer to a Plane.
 *
 * @return Shape_Ptr
 */
Plane_Ptr plane()
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
Sphere_Ptr sphere()
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
Sphere_Ptr glass_sphere()
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
