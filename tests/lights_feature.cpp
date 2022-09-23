/**
 * @file tests/lights_feature.cpp
 * @brief Tests for lights and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/lights.hpp>

#include "catch_helpers.hpp"


SCENARIO("A point light has a position and intensity.", "[lights]")
{
    GIVEN("intensity <- color(1, 1, 1)") {
        auto intensity = color(1, 1, 1);
        AND_GIVEN("position <- point(0, 0, 0)") {
            auto position = point(0, 0, 0);
            WHEN("light <- point_light(position, intensity)") {
                auto light = point_light(position, intensity);
                THEN("light.position = position") {
                    REQUIRE(light->position == position);
                    AND_THEN("light.intensity = intensity") {
                        REQUIRE(light->intensity == intensity);
                    }
                }
            }
        }
    }
}
