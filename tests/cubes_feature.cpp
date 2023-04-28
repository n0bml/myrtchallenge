/**
 * @file tests/cubes_feature.cpp
 * @brief Tests for cubes and support functions.
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


SCENARIO("A ray intersects a cube.", "[cubes]") {
    struct Test_Case {
        Tuple origin;
        Tuple direction;
        double_t t1;
        double_t t2;
    };

    std::vector<Test_Case> test_cases {
        /* +x */     { point( 5,    0.5,  0), vector(-1,  0,  0),  4, 6 },
        /* -x */     { point(-5,    0.5,  0), vector( 1,  0,  0),  4, 6 },
        /* +y */     { point( 0.5,  5,    0), vector( 0, -1,  0),  4, 6 },
        /* -y */     { point( 0.5, -5,    0), vector( 0,  1,  0),  4, 6 },
        /* +z */     { point( 0.5,  0,    5), vector( 0,  0, -1),  4, 6 },
        /* -z */     { point( 0.5,  0,   -5), vector( 0,  0,  1),  4, 6 },
        /* inside */ { point( 0,    0.5,  0), vector( 0,  0,  1), -1, 1 },
    };

    GIVEN("c <- cube()") {
        auto c = cube();

        for (auto&& [origin, direction, t1, t2] : test_cases) {
            WHEN("r <- ray(<origin>, <direction>)") {
                auto r = ray(origin, direction);
                AND_WHEN("xs <- local_intersect(c, r)") {
                    auto xs = c->local_intersect(r);
                    THEN("xs.count = 2") {
                        REQUIRE(xs.size() == 2);
                        AND_THEN("xs[0].t = <t1>") {
                            REQUIRE(xs[0].t == t1);
                            AND_THEN("xs[1] = <t2>") {
                                REQUIRE(xs[1].t == t2);
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("A ray misses a cube.", "[cubes]") {
    struct Test_Case {
        Tuple origin;
        Tuple direction;
    };

    std::vector<Test_Case> test_cases {
        { point(-2,  0,  0), vector(0.2673, 0.5345, 0.8018) },
        { point( 0, -2,  0), vector(0.8018, 0.2673, 0.5345) },
        { point( 0,  0, -2), vector(0.5345, 0.8018, 0.2673) },
        { point( 2,  0,  2), vector( 0,  0, -1) },
        { point( 0,  2,  2), vector( 0, -1,  0) },
        { point( 2,  2,  0), vector(-1,  0,  0) },
    };

    GIVEN("c <- cube()") {
        auto c = cube();

        for (auto&& [origin, direction] : test_cases) {
            WHEN("r <- ray(<origin>, <direction>)") {
                auto r = ray(origin, direction);
                AND_WHEN("xs <- local_intersect(c, r)") {
                    auto xs = c->local_intersect(r);
                    THEN("xs.count = 0") {
                        REQUIRE(xs.size() == 0);
                        REQUIRE(xs.empty());
                    }
                }
            }
        }
    }
}


SCENARIO("The normal on the surface of a cube.", "[cubes]") {
    struct Test_Case {
        Tuple pt;
        Tuple tnormal;
    };

    std::vector<Test_Case> test_cases {
        { point( 1,    0.5, -0.8), vector( 1, 0,  0) },
        { point(-1,   -0.2,  0.9), vector(-1, 0,  0) },
        { point(-0.4,  1,   -0.1), vector( 0, 1,  0) },
        { point( 0.3, -1,   -0.7), vector( 0, 1,  0) },
        { point(-0.6,  0.3,  1),   vector( 0, 0,  1) },
        { point( 0.4,  0.4, -1),   vector( 0, 0, -1) },
        { point( 1,    1,    1),   vector( 1, 0,  0) },
        { point(-1,   -1,   -1),   vector(-1, 0,  0) }
    };

    GIVEN("c <- cube()") {
        auto c = cube();

        for (auto&& [pt, tnormal] : test_cases) {
            WHEN("normal <- local_normal(c, <pt>)") {
                auto normal = c->local_normal_at(pt);
                THEN("normal = <normal>") {
                    REQUIRE(normal == tnormal);
                }
            }
        }
    }
}
