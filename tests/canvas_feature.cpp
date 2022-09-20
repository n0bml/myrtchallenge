/**
 * @file tests/canvas_feature.cpp
 * @brief Tests for canvas and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <algorithm>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/canvas.hpp>


TEST_CASE("Creating a canvas.", "[canvas]")
{
    auto c = canvas(10, 20);
    REQUIRE(c.width == 10);
    REQUIRE(c.height == 20);
    REQUIRE(std::all_of(std::begin(c.pixels), std::end(c.pixels), [](const Color& c) { return c == color(0, 0, 0);}));
}


TEST_CASE("Writing pixels to a canvas.", "[canvas]")
{
    auto c = canvas(10, 20);
    auto red = color(1, 0, 0);
    write_pixel(c, 2, 3, red);
    REQUIRE(pixel_at(c, 2, 3) == red);
}
