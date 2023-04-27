/**
 * @file tests/intersections_feature.cpp
 * @brief Tests for intersections and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../../src/primitives.hpp"
#include <myrtchallenge/rays.hpp>
#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>
#include <myrtchallenge/world.hpp>

#include "catch_helpers.hpp"


SCENARIO("An intersection encapsulates t and object.", "[intersections]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        WHEN("i <- intersection(3.5, s)") {
            auto i = intersection(3.5, s);
            THEN("i.t = 3.5") {
                REQUIRE(i.t == 3.5);
                AND_THEN("i.object = s") {
                    REQUIRE(i.object == s);
                }
            }
        }
    }
}


SCENARIO("Aggregating intersections.", "[intersections]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        AND_GIVEN("i1 <- intersection(1, s)") {
            auto i1 = intersection(1, s);
            AND_GIVEN("i2 <- intersection(2, s)") {
                auto i2 = intersection(2, s);
                WHEN("xs <- intersections(i1, i2)") {
                    auto xs = intersections({i1, i2});
                    THEN("xs.count = 2") {
                        REQUIRE(xs.size() == 2);
                        AND_THEN("xs[0].t = 1") {
                            REQUIRE(xs[0].t == 1);
                            AND_THEN("xs[1].t = 2") {
                                REQUIRE(xs[1].t == 2);
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The hit, when all intersections have positive t.", "[intersections]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        AND_GIVEN("i1 <- intersection(1, s)") {
            auto i1 = intersection(1, s);
            AND_GIVEN("i2 <- intersection(2, s)") {
                auto i2 = intersection(2, s);
                AND_GIVEN("xs <- intersections(i1, i2)") {
                    auto xs = intersections({i1, i2});
                    WHEN("i <- hit(xs)") {
                        auto i = hit(xs);
                        THEN("i = i1") {
                            REQUIRE(i == i1);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The hit, when some intersections have negative t.", "[intersections]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        AND_GIVEN("i1 <- intersection(-1, s)") {
            auto i1 = intersection(-1, s);
            AND_GIVEN("i2 <- intersection(1, s)") {
                auto i2 = intersection(1, s);
                AND_GIVEN("xs <- intersections(i1, i2)") {
                    auto xs = intersections({i1, i2});
                    WHEN("i <- hit(xs)") {
                        auto i = hit(xs);
                        THEN("i = i2") {
                            REQUIRE(i == i2);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The hit, when all intersections have negative t.", "[intersections]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        AND_GIVEN("i1 <- intersection(-2, s)") {
            auto i1 = intersection(-2, s);
            AND_GIVEN("i2 <- intersection(-1, s)") {
                auto i2 = intersection(-1, s);
                AND_GIVEN("xs <- intersections(i1, i2)") {
                    auto xs = intersections({i1, i2});
                    WHEN("i <- hit(xs)") {
                        auto i = hit(xs);
                        THEN("i is nothing") {
                            REQUIRE_FALSE(i.hit());
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The hit is always the lowest non-negative intersection.", "[intersections]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        AND_GIVEN("i1 <- intersection(5, s)") {
            auto i1 = intersection(5, s);
            AND_GIVEN("i2 <- intersection(7, s)") {
                auto i2 = intersection(7, s);
                AND_GIVEN("i3 <- intersection(-3, s)") {
                    auto i3 = intersection(-3, s);
                    AND_GIVEN("i4 <- intersection(2, s)") {
                        auto i4 = intersection(2, s);
                        AND_GIVEN("xs <-intersections(i1, i2, i3, i4)") {
                            auto xs = intersections({i1, i2, i3, i4});
                            WHEN("i <- hit(xs)") {
                                auto i = hit(xs);
                                THEN("i = i4") {
                                    REQUIRE(i == i4);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Precomputing the state of an intersection.", "[intersections]") {
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("shape <- sphere()") {
            auto shape = sphere();
            AND_GIVEN("i <- intersection(4, shape)") {
                auto i = intersection(4, shape);
                WHEN("comps <- prepare_computations(i, r)") {
                    auto comps = prepare_computations(i, r);
                    THEN("comps.t = i.t") {
                        REQUIRE(comps.t == i.t);
                        AND_THEN("comps.object = i.object") {
                            REQUIRE(comps.object == i.object);
                            AND_THEN("comps.point = point(0, 0, -1)") {
                                REQUIRE(comps.point == point(0, 0, -1));
                                AND_THEN("comps.eyev = vector(0, 0, -1)") {
                                    REQUIRE(comps.eyev == vector(0, 0, -1));
                                    AND_THEN("comps.normalv = vector(0, 0, -1)") {
                                        REQUIRE(comps.normalv == vector(0, 0, -1));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The hit, when an intersection occurs on the outside.", "[intersections]") {
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("shape <- sphere()") {
            auto shape = sphere();
            AND_GIVEN("i <- intersection(4, shape)") {
                auto i = intersection(4, shape);
                WHEN("comps <- prepare_computations(i, r)") {
                    auto comps = prepare_computations(i, r);
                    THEN("comps.inside = false") {
                        REQUIRE(comps.inside == false);
                    }
                }
            }
        }
    }
}


SCENARIO("The hit, when an intersection occurs on the inside.", "[intersections]") {
    GIVEN("r <- ray(point(0, 0, 0), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, 0), vector(0, 0, 1));
        AND_GIVEN("shape <- sphere()") {
            auto shape = sphere();
            AND_GIVEN("i <- intersection(1, shape)") {
                auto i = intersection(1, shape);
                WHEN("comps <- prepare_computations(i, r)") {
                    auto comps = prepare_computations(i, r);
                    THEN("comps.point = point(0, 0, 1)") {
                        REQUIRE(comps.point == point(0, 0, 1));
                        AND_THEN("comps.inside = true") {
                            REQUIRE(comps.inside == true);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("shade_hit() is given an intersection in shadow.", "[intersections]") {
    GIVEN("w <- world()") {
        auto w = world();
        AND_GIVEN("w.light <- point_light(point(0, 0, -10), color(1, 1, 1))") {
            w->light = point_light(point(0, 0, -10), color(1, 1, 1));
            AND_GIVEN("s1 <- sphere()") {
                auto s1 = sphere();
                AND_GIVEN("s1 is added to w") {
                    w->objects.push_back(s1);
                    AND_GIVEN("s2 <- sphere() with transform translation(0, 0, 10)") {
                        auto s2 = sphere();
                        s2->transform = translation(0, 0, 10);
                        AND_GIVEN("s2 is added to w") {
                            w->objects.push_back(s2);
                            AND_GIVEN("r <- ray(point(0, 0, 5), vector(0, 0, 1))") {
                                auto r = ray(point(0, 0, 5), vector(0, 0, 1));
                                AND_GIVEN("i <- intersection(4, s2)") {
                                    auto i = intersection(4, s2);
                                    WHEN("comps <- prepare_computations(i, r)") {
                                        auto comps = prepare_computations(i, r);
                                        AND_WHEN("c <- shade_hit(w, comps)") {
                                            auto c = shade_hit(w, comps);
                                            THEN("c = color(0.1, 0.1, 0.1)") {
                                                REQUIRE(c == color(0.1, 0.1, 0.1));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The hit should offset the point.", "[intersections]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("shape <- sphere() with transform translation(0, 0, 1)") {
            auto shape = sphere();
            shape->transform = translation(0, 0, 1);
            AND_GIVEN("i <- intersection(5, shape)") {
                auto i = intersection(5, shape);
                WHEN("comps <- prepare_computations(i, r)") {
                    auto comps = prepare_computations(i, r);
                    THEN("comps.over_point.z < -EPSILON/2") {
                        REQUIRE(comps.over_point.z < 0.00001/2);
                    }
                }
            }
        }
    }
}


SCENARIO("Precomputing the reflection vector.", "[intersections]") {
    GIVEN("shape <- plane()") {
        auto shape = plane();
        AND_GIVEN("r <- ray(point(0, 1, -1), vector(0, -√2/2, √2/2))") {
            auto r = ray(point(0, 1, -1), vector(0, -M_SQRT2 / 2, M_SQRT2 / 2));
            AND_GIVEN("i <- intersection(√2, shape)") {
                auto i = intersection(M_SQRT2, shape);
                WHEN("comps <- prepare_computations(i, r)") {
                    auto comps = prepare_computations(i, r);
                    THEN("comps.reflectv = vector(0, √2/2, √2/2)") {
                        REQUIRE(comps.reflectv == vector(0, M_SQRT2 / 2, M_SQRT2 / 2));
                    }
                }
            }
        }
    }
}


SCENARIO("Finding n1 and n2 at various intersections.", "[intersections]") {
    auto a = glass_sphere();
    a->transform = scaling(2, 2, 2);
    a->material->refractive_index = 1.5;

    auto b = glass_sphere();
    b->transform = translation(0, 0, -0.25);
    b->material->refractive_index = 2.0;

    auto c = glass_sphere();
    c->transform = translation(0, 0, 0.25);
    c->material->refractive_index = 2.5;

    auto r = ray(point(0, 0, -4), vector(0, 0, 1));
    auto xs = intersections({{2, a}, {2.75, b}, {3.25, c}, {4.75, b}, {5.25, c}, {6, a}});

    int index;
    double_t n1, n2;

    GIVEN("index 0 n1 1.0 n2 1.5") {
        int index = 0;
        double_t n1 = 1.0;
        double_t n2 = 1.5;

        WHEN("comps <- prepare_computations(xs[<index>], r, xs)") {
            auto comps = prepare_computations(xs[index], r, xs);
            THEN("comps.n1 = <n1>") {
                REQUIRE(comps.n1 == n1);
                AND_THEN("comps.n2 = <n2>") {
                    REQUIRE(comps.n2 == n2);
                }
            }
        }
    }

    GIVEN("index 1 n1 1.5 n2 2.0") {
        int index = 1;
        double_t n1 = 1.5;
        double_t n2 = 2.0;

        WHEN("comps <- prepare_computations(xs[<index>], r, xs)") {
            auto comps = prepare_computations(xs[index], r, xs);
            THEN("comps.n1 = <n1>") {
                REQUIRE(comps.n1 == n1);
                AND_THEN("comps.n2 = <n2>") {
                    REQUIRE(comps.n2 == n2);
                }
            }
        }
    }

    GIVEN("index 2 n1 2.0 n2 2.5") {
        int index = 2;
        double_t n1 = 2.0;
        double_t n2 = 2.5;

        WHEN("comps <- prepare_computations(xs[<index>], r, xs)") {
            auto comps = prepare_computations(xs[index], r, xs);
            THEN("comps.n1 = <n1>") {
                REQUIRE(comps.n1 == n1);
                AND_THEN("comps.n2 = <n2>") {
                    REQUIRE(comps.n2 == n2);
                }
            }
        }
    }

    GIVEN("index 3 n1 2.5 n2 2.5") {
        int index = 3;
        double_t n1 = 2.5;
        double_t n2 = 2.5;

        WHEN("comps <- prepare_computations(xs[<index>], r, xs)") {
            auto comps = prepare_computations(xs[index], r, xs);
            THEN("comps.n1 = <n1>") {
                REQUIRE(comps.n1 == n1);
                AND_THEN("comps.n2 = <n2>") {
                    REQUIRE(comps.n2 == n2);
                }
            }
        }
    }

    GIVEN("index 4 n1 2.5 n2 1.5") {
        int index = 4;
        double_t n1 = 2.5;
        double_t n2 = 1.5;

        WHEN("comps <- prepare_computations(xs[<index>], r, xs)") {
            auto comps = prepare_computations(xs[index], r, xs);
            THEN("comps.n1 = <n1>") {
                REQUIRE(comps.n1 == n1);
                AND_THEN("comps.n2 = <n2>") {
                    REQUIRE(comps.n2 == n2);
                }
            }
        }
    }

    GIVEN("index 5 n1 1.5 n2 1.0") {
        int index = 5;
        double_t n1 = 1.5;
        double_t n2 = 1.0;

        WHEN("comps <- prepare_computations(xs[<index>], r, xs)") {
            auto comps = prepare_computations(xs[index], r, xs);
            THEN("comps.n1 = <n1>") {
                REQUIRE(comps.n1 == n1);
                AND_THEN("comps.n2 = <n2>") {
                    REQUIRE(comps.n2 == n2);
                }
            }
        }
    }
}


SCENARIO("The under point is offset below the surface.", "[intersections]") {
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("shape <- glass_sphere()...") {
            auto shape = glass_sphere();
            shape->transform = translation(0, 0, 1);
            AND_GIVEN("i <- intersection(5, shape)") {
                auto i = intersection(5, shape);
                AND_GIVEN("xs <- intersections(i)") {
                    auto xs = intersections({i});
                    WHEN("comps <- prepare_computations(i, r, xs)") {
                        auto comps = prepare_computations(i, r, xs);
                        THEN("comps.under_point.z > EPSILON/2") {
                            REQUIRE(comps.under_point.z > EPSILON / 2);
                            AND_THEN("comps.point.z < comps.under_point.z") {
                                REQUIRE(comps.point.z < comps.under_point.z);
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The Schlick approximation under total internal reflection.", "[intersections]") {
    GIVEN("shape <- glass_sphere()") {
        auto shape = glass_sphere();
        AND_GIVEN("r <- ray(point(0, 0, -√2/2), vector(0, 1, 0))") {
            auto r = ray(point(0, 0, -M_SQRT2/2), vector(0, 1, 0));
            AND_GIVEN("xs <- intersections(-√2/2:shape, √2/2:shape)") {
                auto xs = intersections({{-M_SQRT2/2, shape}, {M_SQRT2/2, shape}});
                WHEN("comps <- prepare_computations(xs[1], r, xs)") {
                    auto comps = prepare_computations(xs[1], r, xs);
                    AND_WHEN("reflectance <- schlick(comps)") {
                        auto reflectance = schlick(comps);
                        THEN("reflectance = 1.0") {
                            REQUIRE(reflectance == 1.0);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The Schlick approximation with a perpendicular viewing angle.", "[intersections]") {
    GIVEN("shape <- glass_sphere()") {
        auto shape = glass_sphere();
        AND_GIVEN("r <- ray(point(0, 0, 0), vector(0, 1, 0))") {
            auto r = ray(point(0, 0, 0), vector(0, 1, 0));
            AND_GIVEN("xs <- intersections(-1:shape, 1:shape)") {
                auto xs = intersections({{-1, shape}, {1, shape}});
                WHEN("comps <- prepare_computations(xs[1], r, xs)") {
                    auto comps = prepare_computations(xs[1], r, xs);
                    AND_WHEN("reflectance <- schlick(comps)") {
                        auto reflectance = schlick(comps);
                        THEN("reflectance = 0.04") {
                            REQUIRE(equal(reflectance, 0.04));
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The Schlick approximation with small angle and n2 > n1.", "[intersections]") {
    GIVEN("shape <- glass_sphere()") {
        auto shape = glass_sphere();
        AND_GIVEN("r <- ray(point(0, 0.99, -2), vector(0, 0, 1))") {
            auto r = ray(point(0, 0.99, -2), vector(0, 0, 1));
            AND_GIVEN("xs <- intersections(1.8589:shape)") {
                auto xs = intersections({{1.8589, shape}});
                WHEN("comps <- prepare_computations(xs[0], r, xs)") {
                    auto comps = prepare_computations(xs[0], r, xs);
                    AND_WHEN("reflectance <- schlick(comps)") {
                        auto reflectance = schlick(comps);
                        THEN("reflectance = 0.48873") {
                            REQUIRE(equal(reflectance, 0.48873));
                        }
                    }
                }
            }
        }
    }
}
