/**
 * @file tests/intersections_feature.cpp
 * @brief Tests for intersections and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

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
