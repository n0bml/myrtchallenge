/**
 * @file tests/cylinders_feature.cpp
 * @brief Tests for cylinders and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2023 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/primitives.hpp"
#include <myrtchallenge/rays.hpp>
#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>
#include <myrtchallenge/world.hpp>

#include "catch_helpers.hpp"


SCENARIO("A ray misses a cylinder.", "[cylinders]") {
    struct Test_Case {
        Tuple origin;
        Tuple direction;
    };

    std::vector<Test_Case> test_cases {
        { point(1, 0,  0), vector(0, 1, 0) },
        { point(0, 0,  0), vector(0, 1, 0) },
        { point(0, 0, -5), vector(1, 1, 1) }
    };

    GIVEN("cyl <- cylinder()") {
        auto cyl = cylinder();

        for (auto&& [to, td] : test_cases) {
            WHEN("direction <- normalize(<direction>)") {
                auto direction = normalize(td);
                AND_WHEN("r <- ray(<origin>, direction)") {
                    auto r = ray(to, direction);
                    AND_WHEN("xs <- local_intersect(c, r)") {
                        auto xs = cyl->local_intersect(r);
                        THEN("xs.count = 0") {
                            REQUIRE(xs.size() == 0);
                            REQUIRE(xs.empty());
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("A ray strikes a cylinder.", "[cylinders]") {
    struct Test_Case {
        Tuple origin;
        Tuple direction;
        double_t t0;
        double_t t1;
    };

    std::vector<Test_Case> test_cases {
        { point(1,   0, -5), vector(0,   0,  1),  5,       5       },
        { point(0,   0, -5), vector(0,   0,  1),  4,       6       },
        { point(0.5, 0, -5), vector(0.1, 1,  1),  6.80798, 7.08872 },
    };

    GIVEN("cyl <- cylinder()") {
        auto cyl = cylinder();

        for (auto&& [to, td, t0, t1] : test_cases) {
            WHEN("direction <- normalize(<direction>)") {
                auto direction = normalize(td);
                AND_WHEN("r <- ray(<origin>, direction)") {
                    auto r = ray(to, direction);
                    AND_WHEN("xs <- local_intersect(cyl, r)") {
                        auto xs = cyl->local_intersect(r);
                        THEN("xs.count = 2") {
                            REQUIRE(xs.size() == 2);
                            AND_THEN("xs[0].t = <t0>") {
                                REQUIRE(xs[0].t == t0);
                                AND_THEN("xs[1] = <t1>") {
                                    REQUIRE(xs[1].t == t1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
