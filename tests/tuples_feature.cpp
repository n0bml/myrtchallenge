/**
 * @file tests/tuples_feature.cpp
 * @brief Tests for tuple, point, vector and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <myrtchallenge/colors.hpp>
#include <myrtchallenge/tuples.hpp>

#include "catch_helpers.hpp"


TEST_CASE("A tuple with w=1.0 is a point.", "[tuples]")
{
    auto a = tuple(4.3, -4.2, 3.1, 1.0);
    REQUIRE(a.x == 4.3);
    REQUIRE(a.y == -4.2);
    REQUIRE(a.z == 3.1);
    REQUIRE(a.w == 1.0);
    REQUIRE(a.is_point());
    REQUIRE_FALSE(a.is_vector());
}


TEST_CASE("A tuple with w=0.0 is a vector.", "[tuples]")
{
    auto a = tuple(4.3, -4.2, 3.1, 0.0);
    REQUIRE(a.x == 4.3);
    REQUIRE(a.y == -4.2);
    REQUIRE(a.z == 3.1);
    REQUIRE(a.w == 0.0);
    REQUIRE_FALSE(a.is_point());
    REQUIRE(a.is_vector());
}


TEST_CASE("point() creates tuples with w=1.", "[tuples]")
{
    auto p = point(4, -4, 3);
    REQUIRE(p == tuple(4, -4, 3, 1));
}


TEST_CASE("vector() creates tuples with w=0.", "[tuples]")
{
    auto v = vector(4, -4, 3);
    REQUIRE(v == tuple(4, -4, 3, 0));
}


TEST_CASE("Adding two tuples.", "[tuples]")
{
    auto a1 = tuple(3, -2, 5, 1);
    auto a2 = tuple(-2, 3, 1, 0);
    REQUIRE(a1 + a2 == tuple(1, 1, 6, 1));
}


TEST_CASE("Subtracting two points.", "[tuples]")
{
    auto p1 = point(3, 2, 1);
    auto p2 = point(5, 6, 7);
    REQUIRE(p1 - p2 == vector(-2, -4, -6));
}


TEST_CASE("Subtracting a vector from a point.", "[tuples]")
{
    auto p = point(3, 2, 1);
    auto v = vector(5, 6, 7);
    REQUIRE(p - v == point(-2, -4, -6));
}


TEST_CASE("Subtracting two vectors.", "[tuples]")
{
    auto v1 = vector(3, 2, 1);
    auto v2 = vector(5, 6, 7);
    REQUIRE(v1 - v2 == vector(-2, -4, -6));
}


TEST_CASE("Subtracting a vector from the zero vector.", "[tuples]")
{
    auto zero = vector(0, 0, 0);
    auto v = vector(1, -2, 3);
    REQUIRE(zero - v == vector(-1, 2, -3));
}


TEST_CASE("Negating a tuple.", "[tuples]")
{
    auto a = tuple(1, -2, 3, -4);
    REQUIRE(-a == tuple(-1, 2, -3, 4));
}


TEST_CASE("Mulitplying a tuple by a scalar.", "[tuples]")
{
    auto a = tuple(1, -2, 3, -4);
    REQUIRE(a * 3.5 == tuple(3.5, -7, 10.5, -14));
}


TEST_CASE("Multiplying a tuple by a fraction.", "[tuples]")
{
    auto a = tuple(1, -2, 3, -4);
    REQUIRE(a * 0.5 == tuple(0.5, -1, 1.5, -2));
}


TEST_CASE("Dividing a tuple by a scalar.", "[tuples]")
{
    auto a = tuple(1, -2, 3, -4);
    REQUIRE(a / 2 == tuple(0.5, -1, 1.5, -2));
}


TEST_CASE("Computing the magnitude of vector(1, 0, 0).", "[tuples]")
{
    auto v = vector(1, 0, 0);
    REQUIRE(magnitude(v) == 1);
}


TEST_CASE("Computing the magnitude of vector(0, 1, 0).", "[tuples]")
{
    auto v = vector(0, 1, 0);
    REQUIRE(magnitude(v) == 1);
}


TEST_CASE("Computing the magnitude of vector(0, 0, 1).", "[tuples]")
{
    auto v = vector(0, 0, 1);
    REQUIRE(magnitude(v) == 1);
}


TEST_CASE("Computing the magnitude of vector(1, 2, 3).", "[tuples]")
{
    auto v = vector(1, 2, 3);
    REQUIRE(magnitude(v) == std::sqrt(14));
}


TEST_CASE("Computing the magnitude of vector(-1, -2, -3).", "[tuples]")
{
    auto v = vector(-1, -2, -3);
    REQUIRE(magnitude(v) == std::sqrt(14));
}


TEST_CASE("Normalizing a vector(4, 0, 0) gives (1, 0, 0).", "[tuples]")
{
    auto v = vector(4, 0, 0);
    REQUIRE(normalize(v) == vector(1, 0, 0));
}


TEST_CASE("Normalizing vector(1, 2, 3).", "[tuples]")
{
    auto v = vector(1, 2, 3);
    REQUIRE(normalize(v) == vector(1 / std::sqrt(14), 2 / std::sqrt(14), 3 / std::sqrt(14)));
}


TEST_CASE("The magnitude of a normalized vector.", "[tuples]")
{
    auto v = vector(1, 2, 3);
    auto norm = normalize(v);
    REQUIRE(magnitude(norm) == 1);
}


TEST_CASE("The dot product of two tuples.", "[tuples]")
{
    auto a = vector(1, 2, 3);
    auto b = vector(2, 3, 4);
    REQUIRE(dot(a, b) == 20);
}


TEST_CASE("The cross product of two vectors.", "[tuples]")
{
    auto a = vector(1, 2, 3);
    auto b = vector(2, 3, 4);
    REQUIRE(cross(a, b) == vector(-1, 2, -1));
    REQUIRE(cross(b, a) == vector(1, -2, 1));
}


TEST_CASE("Colors are (red, green, blue) tuples.", "[tuples]")
{
    auto c = color(-0.5, 0.4, 1.7);
    REQUIRE(c.red == -0.5);
    REQUIRE(c.green == 0.4);
    REQUIRE(c.blue == 1.7);
}


TEST_CASE("Adding colors.", "[tuples]")
{
    auto c1 = color(0.9, 0.6, 0.75);
    auto c2 = color(0.7, 0.1, 0.25);
    REQUIRE(c1 + c2 == color(1.6, 0.7, 1.0));
}


TEST_CASE("Subtracting colors.", "[tuples]")
{
    auto c1 = color(0.9, 0.6, 0.75);
    auto c2 = color(0.7, 0.1, 0.25);
    REQUIRE(c1 - c2 == color(0.2, 0.5, 0.5));
}


TEST_CASE("Multiplying a color by a scalar.", "[tuples]")
{
    auto c = color(0.2, 0.3, 0.4);
    REQUIRE(c * 2 == color(0.4, 0.6, 0.8));
}


TEST_CASE("Multiplying colors.", "[tuples]")
{
    auto c1 = color(1, 0.2, 0.4);
    auto c2 = color(0.9, 1, 0.1);
    REQUIRE(c1 * c2 == color(0.9, 0.2, 0.04));
}
