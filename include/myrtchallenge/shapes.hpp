#pragma once

/**
 * @file include/myrtchallenge/shapes.hpp
 * @brief Declaration of shape classes and support functions.
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


struct Shape : std::enable_shared_from_this<Shape>
{
    virtual ~Shape() = default;

    Matrix transform;
    Material_Ptr material;
    std::shared_ptr<Shape> parent;

    bool operator==(const Shape& rhs) const;

    virtual Intersections local_intersect(const Ray& ray) = 0;
    virtual Tuple local_normal_at(const Tuple& point) const = 0;
};


struct Cube : public Shape
{
    Intersections local_intersect(const Ray& ray);
    Tuple local_normal_at(const Tuple& point) const;
};


struct Cylinder : public Shape
{
    double_t minimum{-INFINITY};
    double_t maximum{INFINITY};
    bool closed{false};

    Intersections local_intersect(const Ray& ray);
    Tuple local_normal_at(const Tuple& point) const;

    void intersect_caps(const Ray& ray, Intersections& xs);
};


// a cone is a special form of a cylinder
struct Cone : public Cylinder
{
    Intersections local_intersect(const Ray& ray);
    Tuple local_normal_at(const Tuple& point) const;
};


struct Group : public Shape
{
    std::vector<Shape_Ptr> members;

    Group() : members() {}

    Intersections local_intersect(const Ray& ray);
    Tuple local_normal_at(const Tuple& pt) const;

    bool empty() const;
    bool includes(const Shape_Ptr& shape) const;
};


struct Plane : public Shape
{
    Intersections local_intersect(const Ray& ray);
    Tuple local_normal_at(const Tuple& pt) const;
};


struct Sphere : public Shape
{
    Intersections local_intersect(const Ray& ray);
    Tuple local_normal_at(const Tuple& point) const;
};


using Shape_Ptr = std::shared_ptr<Shape>;
using Cone_Ptr = std::shared_ptr<Cone>;
using Cube_Ptr = std::shared_ptr<Cube>;
using Cylinder_Ptr = std::shared_ptr<Cylinder>;
using Group_Ptr = std::shared_ptr<Group>;
using Plane_Ptr = std::shared_ptr<Plane>;
using Sphere_Ptr = std::shared_ptr<Sphere>;


Cone_Ptr cone();
Cube_Ptr cube();
Cylinder_Ptr cylinder();
Group_Ptr group();
Plane_Ptr plane();
Sphere_Ptr sphere();
Sphere_Ptr glass_sphere();


void add_child(Group_Ptr group, Shape_Ptr shape);
Intersections intersect(Shape_Ptr shape, const Ray& ray);
Tuple normal_at(Shape_Ptr sphere, const Tuple& world_point);
Tuple normal_to_world(const Shape_Ptr& shape, const Tuple& normal);
void set_transform(Shape_Ptr sphere, const Matrix& m);
Tuple world_to_object(const Shape_Ptr& shape, const Tuple& point);
