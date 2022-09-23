/**
 * @file tests/rays_feature.cpp
 * @brief Tests for rays and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/transformations.hpp>
#include <myrtchallenge/rays.hpp>

#include "catch_helpers.hpp"


TEST_CASE("Creating and querying a ray.", "[rays]")
{
    auto origin = point(1, 2, 3);
    auto direction = vector(4, 5, 6);
    auto r = ray(origin, direction);
    REQUIRE(r.origin == origin);
    REQUIRE(r.direction == direction);
}


TEST_CASE("Computing a point from a distance.", "[rays]")
{
    auto r = ray(point(2, 3, 4), vector(1, 0, 0));
    REQUIRE(position(r, 0) == point(2, 3, 4));
    REQUIRE(position(r, 1) == point(3, 3, 4));
    REQUIRE(position(r, -1) == point(1, 3, 4));
    REQUIRE(position(r, 2.5) == point(4.5, 3, 4));
}


SCENARIO("Translating a ray.", "[rays]")
{
    GIVEN("r <- ray(point(1, 2, 3), vector(0, 1, 0))") {
        auto r = ray(point(1, 2, 3), vector(0, 1, 0));
        AND_GIVEN("m <- translation(3, 4, 5)") {
            auto m = translation(3, 4, 5);
            WHEN("r2 <- transform(r, m)") {
                auto r2 = transform(r, m);
                THEN("r2.origin = point(4, 6, 8)") {
                    REQUIRE(r2.origin == point(4, 6, 8));
                    AND_THEN("r2.direction = vector(0, 1, 0)") {
                        REQUIRE(r2.direction == vector(0, 1, 0));
                    }
                }
            }
        }
    }
}


SCENARIO("Scaling a ray.", "[rays]")
{
    GIVEN("r <- ray(point(1, 2, 3), vector(0, 1, 0))") {
        auto r = ray(point(1, 2, 3), vector(0, 1, 0));
        AND_GIVEN("m <- scaling(2, 3, 4)") {
            auto m = scaling(2, 3, 4);
            WHEN("r2 <- transform(r, m)") {
                auto r2 = transform(r, m);
                THEN("r2.origin = point(2, 6, 12)") {
                    REQUIRE(r2.origin == point(2, 6, 12));
                    AND_THEN("r2.direction = vector(0, 3, 0)") {
                        REQUIRE(r2.direction == vector(0, 3, 0));
                    }
                }
            }
        }
    }
}
