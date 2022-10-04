/**
 * @file tests/test_shape.cpp
 * @brief A test shape to be used testing the abstract base class Shape.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <memory>

#include <myrtchallenge/matrices.hpp>
#include <myrtchallenge/materials.hpp>
#include <myrtchallenge/shapes.hpp>


struct Test_Shape : public Shape
{
    Ray saved_ray{point(0, 0, 0), vector(0, 0, 0)};

    Intersections local_intersect(const Ray& ray)
    {
        saved_ray = ray;
        return Intersections{};
    }

    Tuple local_normal_at(const Tuple& point) const
    {
        return vector(point.x, point.y, point.z);
    }
};


Shape_Ptr test_shape()
{
    auto ptr = std::make_shared<Test_Shape>();
    ptr->transform = identity_matrix();
    ptr->material = material();
    return ptr;
}
