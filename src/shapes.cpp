/**
 * @file src/shapes.cpp
 * @brief Definition of shape base class and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022-23 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <algorithm>
#include <array>
#include <cmath>

#include "myrtchallenge/matrices.hpp"
#include "myrtchallenge/shapes.hpp"

#include "primitives.hpp"


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


Bounds Shape::parent_space_bounds_of() const
{
    return local_bounds().transform(transform);
}


/**
 * @brief Add a shape to the group.
 * 
 * @param shape
 */
void add_child(Group_Ptr group, Shape_Ptr shape)
{
    group->members.push_back(shape);
    shape->parent = std::static_pointer_cast<Shape>(group);
}


/**
 * @brief Construct a new empty bounding box.
 */
Bounds bounds()
{
    return Bounds{};
}


/**
 * @brief Counstruct a new bounding box for a shape.
 * 
 * @param shape
 * @return Bounds
 */
Bounds bounds(const Shape_Ptr& shape)
{
    return shape->local_bounds();
}


/**
 * @brief Add a point to the bounding box.
 *
 * @param pt
 */
void Bounds::add(const Tuple& pt)
{
    lower.x = std::min(lower.x, pt.x);
    lower.y = std::min(lower.y, pt.y);
    lower.z = std::min(lower.z, pt.z);

    upper.x = std::max(upper.x, pt.x);
    upper.y = std::max(upper.y, pt.y);
    upper.z = std::max(upper.z, pt.z);
}


/**
 * @brief Add a bounding box to this bounding box.
 *
 * @param box
 */
void Bounds::add(const Bounds& box)
{
    add(box.lower);
    add(box.upper);
}


/**
 * @brief Return true if `point` is inside the bounding box.
 *
 * @param pt
 * @returns True if `pt` is inside this bounding box.
 */
bool Bounds::contains(const Tuple& pt) const
{
    return (lower.x <= pt.x && pt.x <= upper.x)
        && (lower.y <= pt.y && pt.y <= upper.y)
        && (lower.z <= pt.z && pt.z <= upper.z);
}


/**
 * @brief Return true if `box` is inside the bounding box.
 *
 * @param box
 * @returns True if `box` is inside this bounding box.
 */
bool Bounds::contains(const Bounds& box) const
{
    return contains(box.lower) && contains(box.upper);
}


/**
 * @brief Does the ray intersect the bounding box?
 *
 * @param ray
 * @returns True if the ray intersects this bounding box.
 */
bool Bounds::intersects(const Ray& ray) const
{
    auto [xmin, xmax] = check_axis(ray.origin.x, ray.direction.x, lower.x, upper.x);
    auto [ymin, ymax] = check_axis(ray.origin.y, ray.direction.y, lower.y, upper.y);
    auto [zmin, zmax] = check_axis(ray.origin.z, ray.direction.z, lower.z, upper.z);

    auto tmin = std::max({xmin, ymin, zmin});
    auto tmax = std::min({xmax, ymax, zmax});
    if (tmin <= tmax)
        return true;
    return false;
}


/**
 * @brief Split the bounding box into two.
 *
 * @returns Bounds, Bounds the two fractional bounding boxes.
*/
std::tuple<Bounds, Bounds> Bounds::split() const
{
    auto dx = upper.x - lower.x;
    auto dy = upper.y - lower.y;
    auto dz = upper.z - lower.z;

    auto greatest = std::max({dx, dy, dz});

    auto x0 = lower.x;
    auto y0 = lower.y;
    auto z0 = lower.z;

    auto x1 = upper.x;
    auto y1 = upper.y;
    auto z1 = upper.z;

    if (greatest == dx)
        x0 = x1 = x0 + dx / 2;
    else if (greatest == dy)
        y0 = y1 = y0 + dy / 2;
    else
        z0 = z1 = z0 + dz / 2;

    auto mid_min = point(x0, y0, z0);
    auto mid_max = point(x1, y1, z1);

    Bounds left{lower, mid_max};
    Bounds right{mid_min, upper};

    return std::make_tuple(left, right);
}


/**
 * @brief Transform the bounding box with the given transformation matrix.
 *
 * @param mat
 * @returns The transformed bounding box
 */
Bounds Bounds::transform(const Matrix& mat) const
{
    std::array<Tuple, 8> points = {
        lower,
        point(lower.x, lower.y, upper.z),
        point(lower.x, upper.y, lower.z),
        point(lower.x, upper.y, upper.z),
        point(upper.x, lower.y, lower.z),
        point(upper.x, lower.y, upper.z),
        point(upper.x, upper.x, lower.z),
        upper,
    };

    Bounds new_box;
    for (auto const& pt : points)
        new_box.add(mat * pt);
    return new_box;
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
 * @param world_point
 * @return Tuple
 */
Tuple normal_at(Shape_Ptr shape, const Tuple& world_point)
{
    auto local_point = world_to_object(shape, world_point);
    auto local_normal = shape->local_normal_at(local_point);
    return normal_to_world(shape, local_normal);
}


/**
 * @brief Convert a normal from object space to world space.
 * 
 * @param shape
 * @param normal
 * @return Tuple
 */
Tuple normal_to_world(const Shape_Ptr& shape, const Tuple& normal)
{
    auto n = transpose(inverse(shape->transform)) * normal;
    n.w = 0;
    n = normalize(n);
    if (shape->parent) {
        n = normal_to_world(shape->parent, n);
    }
    return n;
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
 * @brief Convert a point from world space to object space.
 * 
 * @param shape - The shape defining the object space.
 * @param point - The point in world space.
 * @return Tuple - The point in ojbect space.
*/
Tuple world_to_object(const Shape_Ptr& shape, const Tuple& point)
{
    auto pt = point;
    if (shape->parent) {
        pt = world_to_object(shape->parent, point);    
    }
    return inverse(shape->transform) * pt;
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


Bounds Cone::local_bounds() const
{
    auto limit = std::max(std::fabs(minimum), std::fabs(maximum));
    return Bounds{point(-limit, minimum, -limit), point(limit, maximum, limit)};
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


Bounds Cube::local_bounds() const
{
    return Bounds{point(-1, -1, -1), point(1, 1, 1)};
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


Bounds Cylinder::local_bounds() const
{
    Bounds box{point(-1, -INFINITY, -1), point(1, INFINITY, 1)};
    if (minimum != -INFINITY)
        box.lower.y = minimum;
    if (maximum != INFINITY)
        box.upper.y = maximum;
    return box;
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
 * @brief Construct and return a shared pointer to an empty Group.
 * 
 * @return Group_Ptr
 */
Group_Ptr group()
{
    auto ptr = std::make_shared<Group>();
    ptr->transform = identity_matrix();
    ptr->material = material();
    return ptr;
}


Bounds Group::local_bounds() const
{
    Bounds box;
    for (auto const& shape : members) {
        auto sbox = shape->parent_space_bounds_of();
        box.add(sbox);
    }
    return box;
}


/**
 * @brief Return the intersections between the ray and the Group.
 *
 * @param ray
 * @return Intersections
 */
Intersections Group::local_intersect(const Ray& ray)
{
    Intersections results;
    for (auto& shape : members) {
        auto xs = intersect(shape, ray);
        results.insert(std::end(results), std::begin(xs), std::end(xs));
    }
    std::sort(std::begin(results), std::end(results));
    return results;
}


/**
 * @brief Return the local normal at the given point.
 *
 * @param pt - unused
 * @return Tuple
 */
Tuple Group::local_normal_at(const Tuple& /*pt*/) const
{
    throw std::runtime_error("Error calling local_normal_at for a group.");
    return vector(0, 1, 0);
}


/**
 * @brief Does the Group have any members?
 * 
 * @return bool
 */
bool Group::empty() const
{
    return members.empty();
}


/**
 * @brief Does the group contain the given shape?
 * 
 * @return bool
 */
bool Group::includes(const Shape_Ptr& shape) const
{
    return std::find(std::begin(members), std::end(members), shape) != std::end(members);
}


/**
 * @brief Construct and return a shared pointer to a Plane.
 *
 * @return Plane_Ptr
 */
Plane_Ptr plane()
{
    auto ptr = std::make_shared<Plane>();
    ptr->transform = identity_matrix();
    ptr->material = material();
    return ptr;
}


Bounds Plane::local_bounds() const
{
    return Bounds{point(-INFINITY, 0, -INFINITY), point(INFINITY, 0, INFINITY)};
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


Bounds Sphere::local_bounds() const
{
    return Bounds{point(-1, -1, -1), point(1, 1, 1)};
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
