/**
 * @file tests/groups_feature.cpp
 * @brief Tests for shape groups and support functions.
 * @author Brendan Leber <n0bml@brendanleber.com>
 * @copyright Copyright 2023 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>

#include "catch_helpers.hpp"
#include "test_shape.hpp"


SCENARIO("Creating a new group.", "[groups]") {
    GIVEN("g <- group()") {
        auto g = group();
        THEN("g.transform = identity_matrix") {
            REQUIRE(g->transform == identity_matrix());
            AND_THEN("g is empty") {
                REQUIRE(g->empty());
            }
        }
    }
}


SCENARIO("A shape has a parent attribute.", "[groups]") {
    GIVEN("s <- test_shape()") {
        auto s = test_shape();
        THEN("s.parent is nothing") {
            REQUIRE(!s->parent);
        }
    }
}


SCENARIO("Adding a child to a group.", "[groups]") {
    GIVEN("g <- group()") {
        auto g = group();
        AND_GIVEN("s <- test_shape()") {
            auto s = test_shape();
            WHEN("add_child(g, s)") {
                add_child(g, s);
                THEN("g is not empty") {
                    REQUIRE(!g->empty());
                    AND_THEN("g includes s") {
                        REQUIRE(g->includes(s));
                        AND_THEN("s.parent = g") {
                            REQUIRE(s->parent == g);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Intersecting a ray with an empty group.", "[groups]") {
    GIVEN("g <- group()") {
        auto g = group();
        AND_GIVEN("r <- ray(point(0, 0, 0), vector(0, 0, 1))") {
            auto r = ray(point(0, 0, 0), vector(0, 0, 1));
            WHEN("xs <- local_intersect(g, r)") {
                auto xs = g->local_intersect(r);
                THEN("xs is empty") {
                    REQUIRE(xs.empty());
                }
            }
        }
    }
}


SCENARIO("Intersecting a ray with a nonempty group.", "[groups]") {
    GIVEN("g <- group()") {
        auto g = group();
        AND_GIVEN("s1 <- sphere()") {
            auto s1 = sphere();
        AND_GIVEN("s2 <- sphere()") {
            auto s2 = sphere();
        AND_GIVEN("set_transform(s2, translation(0, 0, -3))") {
            set_transform(s2, translation(0, 0, -3));
        AND_GIVEN("s3 <- sphere()") {
            auto s3 = sphere();
        AND_GIVEN("set_transform(s3, translation(5, 0, 0))") {
            set_transform(s3, translation(5, 0, 0));
        AND_GIVEN("add_child(g, s1)") {
            add_child(g, s1);
        AND_GIVEN("add_child(g, s2)") {
            add_child(g, s2);
        AND_GIVEN("add_child(g, s3)") {
            add_child(g, s3);
    WHEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_WHEN("xs <- local_intersect(g, r)") {
            auto xs = g->local_intersect(r);
    THEN("xs.count = 4") {
        REQUIRE(xs.size() == 4);
        AND_THEN("xs[0].object = s2") {
            REQUIRE(xs[0].object == s2);
        AND_THEN("xs[1].object = s2") {
            REQUIRE(xs[1].object == s2);
        AND_THEN("xs[2].object = s1") {
            REQUIRE(xs[2].object == s1);
        AND_THEN("xs[3].object = s1") {
            REQUIRE(xs[3].object == s1);
        }}}}}}}}}}}}}}}
    }
}


SCENARIO("Intersecting a transformed group.", "[groups]") {
    GIVEN("g <- group()") {
        auto g = group();
        AND_GIVEN("set_transform(g, scaling(2, 2, 2))") {
            set_transform(g, scaling(2, 2, 2));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
        AND_GIVEN("set_transform(s, translation(5, 0, 0))") {
            set_transform(s, translation(5, 0, 0));
        AND_GIVEN("add_child(g, s)") {
            add_child(g, s);
    WHEN("r <- ray(point(10, 0, -10), vector(0, 0, 1))") {
        auto r = ray(point(10, 0, -10), vector(0, 0, 1));
        AND_WHEN("xs <- intersect(g, r)") {
            auto xs = intersect(g, r);
    THEN("xs.count = 2") {
        REQUIRE(xs.size() == 2);
    }}}}}}}}
}