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


SCENARIO("Normal vector on a cylinder.", "[cylinders]") {
    struct Test_Case {
        Tuple pt;
        Tuple tnormal;
    };

    std::vector<Test_Case> test_cases {
        { point( 1,  0,  0), vector( 1, 0, 0) },
        { point( 0,  5, -1), vector( 0, 0, 1) },
        { point( 0, -2,  1), vector( 0, 0, 1) },
        { point(-1,  1,  0), vector(-1, 0, 0) },
    };

    GIVEN("cyl <- cylinder()") {
        auto cyl = cylinder();

        for (auto&& [pt, tnormal] : test_cases) {
            WHEN("normal <- local_normal_at(cyl, <pt>)") {
                auto normal = cyl->local_normal_at(pt);
                THEN("normal = <normal>") {
                    REQUIRE(normal == tnormal);
                }
            }
        }
    }
}


SCENARIO("The default minimum and maximum for a cylinder.", "[cylinders]") {
    GIVEN("cyl <- cylinder()") {
        auto cyl = std::dynamic_pointer_cast<Cylinder>(cylinder());
        THEN("cyl.minimum = -infinity") {
            REQUIRE(cyl->minimum == -INFINITY);
            AND_THEN("cyl.maximum = infinity") {
                REQUIRE(cyl->maximum == INFINITY);
            }
        }
    }
}


SCENARIO("Intersecting a constrained cylinder.", "[cylinders]") {
    struct Test_Case {
        Tuple tpoint;
        Tuple tdirection;
        size_t count;
    };

    std::vector<Test_Case> test_cases {
        { point(0, 1.5,  0), vector(0.1, 1, 0), 0 },
        { point(0, 3,   -5), vector(0,   0, 1), 0 },
        { point(0, 0,   -5), vector(0,   0, 1), 0 },
        { point(0, 2,   -5), vector(0,   0, 1), 0 },
        { point(0, 1,   -5), vector(0,   0, 1), 0 },
        { point(0, 1.5, -2), vector(0,   0, 1), 2 },
    };

    GIVEN("cyl <- cylinder()") {
        auto cyl = std::dynamic_pointer_cast<Cylinder>(cylinder());
        AND_GIVEN("cyl.minimum <- 1") {
            cyl->minimum = 1;
            AND_GIVEN("cyl.maximum <- 2") {
                cyl->maximum = 2;
                for (auto&& [tpt, td, tc] : test_cases) {
                    WHEN("direction <- normalize(<direction>)") {
                        auto direction = normalize(td);
                        AND_WHEN("r <- ray(<pt>, direction)") {
                            auto r = ray(tpt, direction);
                            AND_WHEN("xs <- local_intersect(cyl, r)") {
                                auto xs = cyl->local_intersect(r);
                                THEN("xs.count = <count>") {
                                    REQUIRE(xs.size() == tc);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
