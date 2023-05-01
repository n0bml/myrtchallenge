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


Shape_Ptr cone();
Shape_Ptr cube();
Shape_Ptr cylinder();
Shape_Ptr plane();
Shape_Ptr sphere();
Shape_Ptr glass_sphere();


Intersections intersect(Shape_Ptr sphere, const Ray& ray);
Tuple normal_at(Shape_Ptr sphere, const Tuple& point);
void set_transform(Shape_Ptr sphere, const Matrix& m);
