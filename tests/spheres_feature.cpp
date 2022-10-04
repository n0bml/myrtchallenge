/**
 * @file tests/spheres_feature.cpp
 * @brief Tests for spheres and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>

#include "catch_helpers.hpp"


SCENARIO("A ray intersects a sphere at two points.", "[spheres]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
            WHEN("xs <- local_intersect(s, r)") {
                auto xs = s->local_intersect(r);
                THEN("xs.count = 2") {
                    REQUIRE(xs.size() == 2);
                    AND_THEN("xs[0].t = 4.0") {
                        REQUIRE(xs[0].t == 4.0);
                        AND_THEN("xs[1].t = 6.0") {
                            REQUIRE(xs[1].t == 6.0);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("A ray intersects a sphere at a tangent.", "[spheres]")
{
    GIVEN("r <- ray(point(0, 1, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 1, -5), vector(0, 0, 1));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
            WHEN("xs <- local_intersect(s, r)") {
                auto xs = s->local_intersect(r);
                THEN("xs.count = 2") {
                    REQUIRE(xs.size() == 2);
                    AND_THEN("xs[0].t = 5.0") {
                        REQUIRE(xs[0].t == 5.0);
                        AND_THEN("xs[1].t = 5.0") {
                            REQUIRE(xs[1].t == 5.0);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("A ray misses a sphere.", "[spheres]")
{
    GIVEN("r <- ray(point(0, 2, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 2, -5), vector(0, 0, 1));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
            WHEN("xs <- local_intersect(s, r)") {
                auto xs = s->local_intersect(r);
                THEN("xs.count = 0") {
                    REQUIRE(xs.empty());
                }
            }
        }
    }
}


SCENARIO("A ray originates inside a sphere.", "[spheres]")
{
    GIVEN("r <- ray(point(0, 0, 0), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, 0), vector(0, 0, 1));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
            WHEN("xs <- local_intersect(s, r)") {
                auto xs = s->local_intersect(r);
                THEN("xs.count = 2") {
                    REQUIRE(xs.size() == 2);
                    AND_THEN("xs[0].t = -1.0") {
                        REQUIRE(xs[0].t == -1.0);
                        AND_THEN("xs[1].t = 1.0") {
                            REQUIRE(xs[1].t == 1.0);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("A sphere is behind a ray.", "[spheres]")
{
    GIVEN("r <- ray(point(0, 0, 5), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, 5), vector(0, 0, 1));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
            WHEN("xs <- local_intersect(s, r)") {
                auto xs = s->local_intersect(r);
                THEN("xs.count = 2") {
                    REQUIRE(xs.size() == 2);
                    AND_THEN("xs[0].t = -6.0") {
                        REQUIRE(xs[0].t == -6.0);
                        AND_THEN("xs[1].t = -4.0") {
                            REQUIRE(xs[1].t == -4.0);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Intersect sets the object on the intersection.", "[spheres]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
            WHEN("xs <- local_intersect(s, r)") {
                auto xs = s->local_intersect(r);
                THEN("xs.count = 2") {
                    REQUIRE(xs.size() == 2);
                    AND_THEN("xs[0].object = s") {
                        REQUIRE(xs[0].object == s);
                        AND_THEN("xs[1].object = s") {
                            REQUIRE(xs[1].object == s);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Intersecting a scaled sphere with a ray.", "[spheres]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
            WHEN("set_transform(s, scaling(2, 2, 2))") {
                set_transform(s, scaling(2, 2, 2));
                AND_WHEN("xs <- intersect(s, r)") {
                    auto xs = intersect(s, r);
                    THEN("xs.count = 2") {
                        REQUIRE(xs.size() == 2);
                        AND_THEN("xs[0].t = 3") {
                            REQUIRE(xs[0].t == 3);
                            AND_THEN("xs[1].t = 7") {
                                REQUIRE(xs[1].t == 7);
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Intersecting a translated sphere with a ray.", "[spheres]")
{
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
            WHEN("set_transform(s, translation(5, 0, 0))") {
                set_transform(s, translation(5, 0, 0));
                AND_WHEN("xs <- intersect(s, r)") {
                    auto xs = intersect(s, r);
                    THEN("xs.count = 0") {
                        REQUIRE(xs.empty());
                    }
                }
            }
        }
    }
}


SCENARIO("The normal on a sphere at a point on the x axis.", "[spheres]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        WHEN("n <- normal_at(s, point(1, 0, 0))") {
            auto n = normal_at(s, point(1, 0, 0));
            THEN("n = vector(1, 0, 0)") {
                REQUIRE(n == vector(1, 0, 0));
            }
        }
    }
}


SCENARIO("The normal on a sphere at a point on the y axis.", "[spheres]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        WHEN("n <- normal_at(s, point(0, 1, 0))") {
            auto n = normal_at(s, point(0, 1, 0));
            THEN("n = vector(0, 1, 0)") {
                REQUIRE(n == vector(0, 1, 0));
            }
        }
    }
}


SCENARIO("The normal on a sphere at a point on the z axis.", "[spheres]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        WHEN("n <- normal_at(s, point(0, 0, 1))") {
            auto n = normal_at(s, point(0, 0, 1));
            THEN("n = vector(0, 0, 1)") {
                REQUIRE(n == vector(0, 0, 1));
            }
        }
    }
}


SCENARIO("The normal on a spehre at a non-axial point.", "[spheres]")
{
    WHEN("s <- sphere()") {
        auto s = sphere();
        WHEN("n <- normal_at(s, point(√3/3, √3/3, √3/3))") {
            auto n = normal_at(s, point(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3));
            THEN("n = vector(√3/3, √3/3, √3/3)") {
                REQUIRE(n == vector(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3));
            }
        }
    }
}


SCENARIO("The normal is a normalized vector.", "[spheres]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        WHEN("n <- normal_at(s, point(√3/3, √3/3, √3/3)") {
            auto n = normal_at(s, point(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3));
            THEN("n = normalize(n)") {
                REQUIRE(n == normalize(n));
            }
        }
    }
}


SCENARIO("Computing the normal on a translated sphere.", "[spheres]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        AND_GIVEN("set_transform(s, translation(0, 1, 0))") {
            set_transform(s, translation(0, 1, 0));
            WHEN("n <- normal_at(s, point(0, 1.70711, -0.70711))") {
                auto n = normal_at(s, point(0, 1.70711, -0.70711));
                THEN("n = vector(0.070711, -0.70711)") {
                    REQUIRE(n == vector(0, 0.70711, -0.70711));
                }
            }
        }
    }
}


SCENARIO("Computing the normal on a transformed sphere.", "[spheres]")
{
    GIVEN("s <- sphere()") {
        auto s = sphere();
        AND_GIVEN("m <- scaling(1, 0.5, 1) * rotation_z(π/5).") {
            auto m = scaling(1, 0.5, 1) * rotation_z(M_PI / 5);
            AND_GIVEN("set_transform(s, m)") {
                set_transform(s, m);
                WHEN("n <- normal_at(s, point(0, √2/2, -√2/2))") {
                    auto n = normal_at(s, point(0, std::sqrt(2) / 2, -std::sqrt(2) / 2));
                    THEN("n = vector(0, 0.97014, -0.24254)") {
                        REQUIRE(n == vector(0, 0.97014, -0.24254));
                    }
                }
            }
        }
    }
}
