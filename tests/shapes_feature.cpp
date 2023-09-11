/**
 * @file tests/shapes_feature.cpp
 * @brief Tests for shape base class and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>

#include "catch_helpers.hpp"
#include "test_shape.hpp"


SCENARIO("The default transformation.", "[shapes]") {
    GIVEN("s <- test_shape()") {
        auto s = test_shape();
        THEN("s.transform = identity_matrix") {
            REQUIRE(s->transform == identity_matrix());
        }
    }
}


SCENARIO("Assigning a transformation.", "[shapes]") {
    GIVEN("s <- test_shape()") {
        auto s = test_shape();
        WHEN("set_transform(s, translation(2, 3, 4))") {
            set_transform(s, translation(2, 3, 4));
            THEN("s.transform = translation(2, 3, 4)") {
                REQUIRE(s->transform == translation(2, 3, 4));
            }
        }
    }
}


SCENARIO("The default material.", "[shapes]") {
    GIVEN("s <- test_shape()") {
        auto s = test_shape();
        WHEN("m <- s.material") {
            auto m = s->material;
            THEN("m = material()") {
                REQUIRE(*m == *material());
            }
        }
    }
}


SCENARIO("Assigning a material.", "[shapes]") {
    GIVEN("s <- test_shape()") {
        auto s = test_shape();
        AND_GIVEN("m <- material()") {
            auto m = material();
            AND_GIVEN("m.ambient <- 1") {
                m->ambient = 1;
                WHEN("s.material <- m") {
                    s->material = m;
                    THEN("s.material = m") {
                        REQUIRE(*s->material == *m);
                    }
                }
            }
        }
    }
}


SCENARIO("Intersecting a scaled shape with a ray.", "[shapes]") {
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("s <- test_shape()") {
            auto s = test_shape();
            WHEN("set_transform(s, scaling(2, 2, 2)") {
                set_transform(s, scaling(2, 2, 2));
                AND_WHEN("xs <- intersect(s, r)") {
                    auto xs = intersect(s, r);
                    THEN("s.saved_ray.origin = point(0, 0, -2.5)") {
                        auto ts = std::dynamic_pointer_cast<Test_Shape>(s);
                        REQUIRE(ts->saved_ray.origin == point(0, 0, -2.5));
                        AND_THEN("s.saved_ray.direction = vector(0, 0, 0.5)") {
                            REQUIRE(ts->saved_ray.direction == vector(0, 0, 0.5));
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Intersecting a translated shape with a ray.", "[shapes]") {
    GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1)") {
        auto r = ray(point(0, 0, -5), vector(0, 0, 1));
        AND_GIVEN("s <- test_shape()") {
            auto s = test_shape();
            WHEN("set_transform(s, translation(5, 0, 0))") {
                set_transform(s, translation(5, 0, 0));
                AND_WHEN("xs <- intersect(s, r)") {
                    auto xs = intersect(s, r);
                    THEN("s.saved_ray.origin = point(-5, 0, -5)") {
                        auto ts = std::dynamic_pointer_cast<Test_Shape>(s);
                        REQUIRE(ts->saved_ray.origin == point(-5, 0, -5));
                        AND_THEN("s.saved_ray.direction = vector(0, 0, 1)") {
                            REQUIRE(ts->saved_ray.direction == vector(0, 0, 1));
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Computing the normal on a translated shape.", "[shapes]") {
    GIVEN("s <- test_shape()") {
        auto s = test_shape();
        WHEN("set_transform(s, translation(0, 1, 0))") {
            set_transform(s, translation(0, 1, 0));
            AND_WHEN("n <- normal_at(s, point(0, 1.70711, -0.70711))") {
                auto n = normal_at(s, point(0, 1.70711, -0.70711));
                THEN("n = vector(0, 0.70711, -0.70711)") {
                    REQUIRE(n == vector(0, 0.70711, -0.70711));
                }
            }
        }
    }
}


SCENARIO("Computing the normal on a transformed shape.", "[shapes]") {
    GIVEN("s <- test_shape()") {
        auto s = test_shape();
        AND_GIVEN("m <- scaling(1, 0.5, 1)") {
            auto m = scaling(1, 0.5, 1);
            WHEN("set_transform(s, m)") {
                set_transform(s, m);
                AND_WHEN("n <- normal_at(s, point(0, √2/2, -√2/2))") {
                    auto n = normal_at(s, point(0, M_SQRT2 / 2, -M_SQRT2 / 2));
                    THEN("n = vector(0, 0.97014, -0.24254)") {
                        REQUIRE(n == vector(0, 0.97014, -0.24254));
                    }
                }
            }
        }
    }
}


SCENARIO("Converting a point from world to object space.", "[shapes]") {
    GIVEN("g1 <- group()") {
        auto g1 = group();
        AND_GIVEN("set_transform(g1, rotation_y(pi/2))") {
            set_transform(g1, rotation_y(M_PI_2));
        AND_GIVEN("g2 <- group()") {
            auto g2 = group();
        AND_GIVEN("set_transform(g2, scaling(2, 2, 2))") {
            set_transform(g2, scaling(2, 2, 2));
        AND_GIVEN("add_child(g1, g2)") {
            add_child(g1, g2);
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
        AND_GIVEN("set_transform(s, translation(5, 0, 0))") {
            set_transform(s, translation(5, 0, 0));
        AND_GIVEN("add_child(g2, s)") {
            add_child(g2, s);
    WHEN("p <- world_to_object(s, point(-2, 0, -10))") {
        auto p = world_to_object(s, point(-2, 0, -10));
    THEN("p = point(0, 0, -1)") {
        REQUIRE(p == point(0, 0, -1));
    }}}}}}}}}}
}


SCENARIO("Converting a normal from object to world space.", "[shapes]") {
    GIVEN("g1 <- group()") {
        auto g1 = group();
        AND_GIVEN("set_transform(g1, rotation_y(pi/2))") {
            set_transform(g1, rotation_y(M_PI_2));
        AND_GIVEN("g2 <- group()") {
            auto g2 = group();
        AND_GIVEN("set_transform(g2, scaling(1, 2, 3))") {
            set_transform(g2, scaling(1, 2, 3));
        AND_GIVEN("add_child(g1, g2)") {
            add_child(g1, g2);
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
        AND_GIVEN("set_transform(s, translation(5, 0, 0))") {
            set_transform(s, translation(5, 0, 0));
        AND_GIVEN("add_child(g2, s)") {
            add_child(g2, s);
    WHEN("n <- normal_to_world(s, vector(√3/3, √3/3, √3/3))") {
        auto n = normal_to_world(s, vector(std::sqrt(3) / 3, std::sqrt(3) / 3, std::sqrt(3) / 3));
    THEN("n = vector(0.2857, 0.4286, -0.8571)") {
        REQUIRE(n == vector(0.28571, 0.42857, -0.85714));
    }}}}}}}}}}
}


SCENARIO("Finding the normal on a child object.", "[shapes]") {
    GIVEN("g1 <- group()") {
        auto g1 = group();
        AND_GIVEN("set_transform(g1, rotation(pi/2))") {
            set_transform(g1, rotation_y(M_PI_2));
        AND_GIVEN("g2 <- group()") {
            auto g2 = group();
        AND_GIVEN("set_transform(g2, scaling(1, 2, 3))") {
            set_transform(g2, scaling(1, 2, 3));
        AND_GIVEN("add_child(g1, g2)") {
            add_child(g1, g2);
        AND_GIVEN("s <- sphere()") {
            auto s = sphere();
        AND_GIVEN("set_transform(s, translation(5, 0, 0))") {
            set_transform(s, translation(5, 0, 0));
        AND_GIVEN("add_child(g2, s)") {
            add_child(g2, s);
    WHEN("n <- normal_at(s, point(1.7321, 1.1547, -5.5774))") {
        auto n = normal_at(s, point(1.7321, 1.1547, -5.5774));
    THEN("n = vector(0.2857, 0.4286, -0.8571)") {
        REQUIRE(n == vector(0.28570, 0.42854, -0.85716));
    }}}}}}}}}}
}