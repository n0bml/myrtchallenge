/**
 * @file tests/planes_feature.cpp
 * @brief Tests for planes and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>

#include "catch_helpers.hpp"


SCENARIO("The normal of a plane is constant everywhere.", "[planes]") {
    GIVEN("p <- plane()") {
        auto p = plane();
        WHEN("n1 <- local_normal_at(p, point(0, 0, 0))") {
            auto n1 = p->local_normal_at(point(0, 0, 0));
            AND_WHEN("n2 <- local_normal_at(p, point(10, 0, -10))") {
                auto n2 = p->local_normal_at(point(10, 0, -10));
                AND_WHEN("n3 <- local_normal_at(p, point(-5, 0, 150))") {
                    auto n3 = p->local_normal_at(point(-5, 0, 150));
                    THEN("n1 = vector(0, 1, 0)") {
                        REQUIRE(n1 == vector(0, 1, 0));
                        AND_THEN("n2 = vector(0, 1, 0)") {
                            REQUIRE(n2 == vector(0, 1, 0));
                            AND_THEN("n3 = vector(0, 1, 0)") {
                                REQUIRE(n3 == vector(0, 1, 0));
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Intersect a ray parallel to the plane.", "[planes]") {
    GIVEN("p <- plane()") {
        auto p = plane();
        AND_GIVEN("r <- ray(point(0, 10, 0), vector(0, 0, 1))") {
            auto r = ray(point(0, 10, 0), vector(0, 0, 1));
            WHEN("xs <- local_intersect(p, r)") {
                auto xs = p->local_intersect(r);
                THEN("xs is empty") {
                    REQUIRE(xs.empty());
                }
            }
        }
    }
}


SCENARIO("Intersect with a coplanar ray.", "[planes]") {
    GIVEN("p <- plane()") {
        auto p = plane();
        AND_GIVEN("r <- ray(point(0, 0, 0), vector(0, 0, 1))") {
            auto r = ray(point(0, 0, 0), vector(0, 0, 1));
            WHEN("xs <- local_intersect(p, r)") {
                auto xs = p->local_intersect(r);
                THEN("xs is empty") {
                    REQUIRE(xs.empty());
                }
            }
        }
    }
}


SCENARIO("A ray intersecting a plane from above.", "[planes]") {
    GIVEN("p <- plane()") {
        auto p = plane();
        AND_GIVEN("r <- ray(point(0, 1, 0), vector(0, -1, 0))") {
            auto r = ray(point(0, 1, 0), vector(0, -1, 0));
            WHEN("xs <- local_intersect(p, r)") {
                auto xs = p->local_intersect(r);
                THEN("xs.count = 1") {
                    REQUIRE(xs.size() == 1);
                    AND_THEN("xs[0].t = 1") {
                        REQUIRE(xs[0].t == 1);
                        AND_THEN("xs[0].object = p") {
                            REQUIRE(xs[0].object.get() == p.get());
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("A ray intersecting a plane from below.", "[planes]") {
    GIVEN("p <- plane()") {
        auto p = plane();
        AND_GIVEN("r <- ray(point(0, -1, 0), vector(0, 1, 0))") {
            auto r = ray(point(0, -1, 0), vector(0, 1, 0));
            WHEN("xs <- local_intersect(p, r)") {
                auto xs = p->local_intersect(r);
                THEN("xs.count = 1") {
                    REQUIRE(xs.size() == 1);
                    AND_THEN("xs[0].t = 1") {
                        REQUIRE(xs[0].t == 1);
                        AND_THEN("xs[0].object = p") {
                            REQUIRE(xs[0].object.get() == p.get());
                        }
                    }
                }
            }
        }
    }
}
