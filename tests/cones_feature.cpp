/**
 * @file tests/cones_feature.cpp
 * @brief Tests for cones and support functions.
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


SCENARIO("Intersecting a cone with a ray.", "[cones]") {
    GIVEN("shape <- cone()") {
        auto shape = cone();

        WHEN("xs <- shape.local_intersection(ray(point(0, 0, -5), normalize(vector(0, 0, 1))))") {
            auto xs = shape->local_intersect(ray(point(0, 0, -5), normalize(vector(0, 0, 1))));
            THEN("x[0].t = 5 and x[1].t = 5") {
                REQUIRE(equal(xs[0].t, 5));
                REQUIRE(equal(xs[1].t, 5));
            }
        }

        WHEN("xs <- shape.local_intersection(ray(point(0, 0, -5), normalize(vector(1, 1, 1))))") {
            auto xs = shape->local_intersect(ray(point(0, 0, -5), normalize(vector(1, 1, 1))));
            THEN("x[0].t = 8.66025 and x[1].t = 8.66025") {
                REQUIRE(equal(xs[0].t, 8.66025));
                REQUIRE(equal(xs[1].t, 8.66025));
            }
        }

        WHEN("xs <- shape.local_intersection(ray(point(1, 1, -5), normalize(vector(-0.5, -1, 1))))") {
            auto xs = shape->local_intersect(ray(point(1, 1, -5), normalize(vector(-0.5, -1, 1))));
            THEN("x[0].t = 4.55006 and x[1].t = 49.44994") {
                REQUIRE(equal(xs[0].t, 4.55006));
                REQUIRE(equal(xs[1].t, 49.44994));
            }
        }
    }
}


SCENARIO("Intersecting a cone with a ray parallel to one of its halves.", "[cones]") {
    GIVEN("shape <- cone()") {
        auto shape = cone();
        AND_GIVEN("direction <- normalize(vector(0, 1, 1))") {
            auto direction = normalize(vector(0, 1, 1));
            AND_GIVEN("r <- ray(point(0, 0, -1), direction)") {
                auto r = ray(point(0, 0, -1), direction);
                WHEN("xs <- local_intersect(shape, r)") {
                    auto xs = shape->local_intersect(r);
                    THEN("xs.count = 1") {
                        REQUIRE(xs.size() == 1);
                        AND_THEN("xs[0].t = 0.35355") {
                            REQUIRE(equal(xs[0].t, 0.35355));
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Intersecting a cone's end caps.", "[cones]") {
    GIVEN("shape <- cone()") {
        auto shape = std::dynamic_pointer_cast<Cone>(cone());
        shape->minimum = 1;
        shape->maximum = 2;
        shape->closed = true;

        WHEN("xs <- local_intersect(shape, ray(point(0, 0, -5), normalize(vector(0, 1, 0))))") {
            auto xs = shape->local_intersect(ray(point(0, 0, -5), normalize(vector(0, 1, 0))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(shape, ray(point(0, 0, -0.25), normalize(vector(0, 1, 1))))") {
            auto xs = shape->local_intersect(ray(point(0, 0, -0.25), normalize(vector(0, 1, 1))));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
        }

        WHEN("xs <- local_intersect(shape, ray(point(0, 0, -0.25), normalize(vector(0, 1, 0))))") {
            auto xs = shape->local_intersect(ray(point(0, 0, -0.25), normalize(vector(0, 1, 0))));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
        }
    }
}


SCENARIO("Computing the normal vector on a cone.", "[cones]") {
    GIVEN("shape <- cone()") {
        auto shape = cone();

        WHEN("n <- local_normal_at(shape, point(0, 0, 0))") {
            auto n = shape->local_normal_at(point(0, 0, 0));
            THEN("n = vector(0, 0, 0)") {
                REQUIRE(n == vector(0, 0, 0));
            }
        }

        WHEN("n <- local_normal_at(shape, point(1, 1, 1))") {
            auto n = shape->local_normal_at(point(1, 1, 1));
            THEN("n = vector(1, -√2, 1)") {
                REQUIRE(n == vector(1, -M_SQRT2, 1));
            }
        }

        WHEN("n <- local_normal_at(shape, point(-1, -1, 0))") {
            auto n = shape->local_normal_at(point(-1, -1, 0));
            THEN("n = vector(-1, 1, 0)") {
                REQUIRE(n == vector(-1, 1, 0));
            }
        }
    }
}
