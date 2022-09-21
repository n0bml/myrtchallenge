/**
 * @file tests/spheres_feature.cpp
 * @brief Tests for spheres and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/rays.hpp>
#include <myrtchallenge/spheres.hpp>
#include <myrtchallenge/transformations.hpp>

#include "catch_helpers.hpp"


TEST_CASE("A ray intersects a sphere at two points.", "[spheres]")
{
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersect(s, r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].t == 4.0);
    REQUIRE(xs[1].t == 6.0);
}


TEST_CASE("A ray intersects a sphere at a tangent.", "[spheres]")
{
    auto r = ray(point(0, 1, -5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersect(s, r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].t == 5.0);
    REQUIRE(xs[1].t == 5.0);
}


TEST_CASE("A ray misses a sphere.", "[spheres]")
{
    auto r = ray(point(0, 2, -5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersect(s, r);
    REQUIRE(xs.empty());
    REQUIRE(xs.size() == 0);
}


TEST_CASE("A ray originates inside a sphere.", "[spheres]")
{
    auto r = ray(point(0, 0, 0), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersect(s, r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].t == -1.0);
    REQUIRE(xs[1].t == 1.0);
}


TEST_CASE("A sphere is behind a ray.", "[spheres]")
{
    auto r = ray(point(0, 0, 5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersect(s, r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].t == -6.0);
    REQUIRE(xs[1].t == -4.0);
}


TEST_CASE("Intersect sets the object on the intersection.", "[spheres]")
{
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersect(s, r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].object == s);
    REQUIRE(xs[1].object == s);
}


TEST_CASE("A sphere's default transformation.", "[spheres]")
{
    auto s = sphere();
    REQUIRE(s->transform == identity_matrix());
}


TEST_CASE("Changing a sphere's transformation.", "[spheres]")
{
    auto s = sphere();
    auto t = translation(2, 3, 4);
    set_transform(s, t);
    REQUIRE(s->transform == t);
}


TEST_CASE("Intersecting a scaled sphere with a ray.", "[spheres]")
{
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    set_transform(s, scaling(2, 2, 2));
    auto xs = intersect(s, r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].t == 3);
    REQUIRE(xs[1].t == 7);
}


TEST_CASE("Intersecting a translated sphere with a ray.", "[spheres]")
{
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    set_transform(s, translation(5, 0, 0));
    auto xs = intersect(s, r);
    REQUIRE(xs.empty());
}
