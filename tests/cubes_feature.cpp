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
    GIVEN("c <- cube()") {
        auto c = cube();

        WHEN("xs <- local_intersect(c, ray(point(5, 0.5, 0), vector(-1, 0, 0)))") {
            auto xs = c->local_intersect(ray(point(5, 0.5, 0), vector(-1, 0, 0)));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
            THEN("xs[0].t = 4") {
                REQUIRE(xs[0].t == 4);
            }
            THEN("xs[1].t = 6") {
                REQUIRE(xs[1].t == 6);
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(-5, 0.5, 0), vector(1, 0, 0)))") {
            auto xs = c->local_intersect(ray(point(-5, 0.5, 0), vector(1, 0, 0)));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
            THEN("xs[0].t = 4") {
                REQUIRE(xs[0].t == 4);
            }
            THEN("xs[1].t = 6") {
                REQUIRE(xs[1].t == 6);
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(0.5, 5, 0), vector(0, -1, 0)))") {
            auto xs = c->local_intersect(ray(point(0.5, 5, 0), vector(0, -1, 0)));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
            THEN("xs[0].t = 4") {
                REQUIRE(xs[0].t == 4);
            }
            THEN("xs[1].t = 6") {
                REQUIRE(xs[1].t == 6);
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(0.5, -5, 0), vector(0, 1, 0)))") {
            auto xs = c->local_intersect(ray(point(0.5, -5, 0), vector(0, 1, 0)));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
            THEN("xs[0].t = 4") {
                REQUIRE(xs[0].t == 4);
            }
            THEN("xs[1].t = 6") {
                REQUIRE(xs[1].t == 6);
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(0.5, 0, 5), vector(0, 0, -1)))") {
            auto xs = c->local_intersect(ray(point(0.5, 0, 5), vector(0, 0, -1)));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
            THEN("xs[0].t = 4") {
                REQUIRE(xs[0].t == 4);
            }
            THEN("xs[1].t = 6") {
                REQUIRE(xs[1].t == 6);
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(0.5, 0, -5), vector(0, 0, 1)))") {
            auto xs = c->local_intersect(ray(point(0.5, 0, -5), vector(0, 0, 1)));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
            THEN("xs[0].t = 4") {
                REQUIRE(xs[0].t == 4);
            }
            THEN("xs[1].t = 6") {
                REQUIRE(xs[1].t == 6);
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(0, 0.5, 0), vector(0, 0, 1)))") {
            auto xs = c->local_intersect(ray(point(0, 0.5, 0), vector(0, 0, 1)));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
            THEN("xs[0].t = -1") {
                REQUIRE(xs[0].t == -1);
            }
            THEN("xs[1].t = 1") {
                REQUIRE(xs[1].t == 1);
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
    };

    GIVEN("c <- cube()") {
        auto c = cube();

        WHEN("xs <- local_intersect(c, ray(point(-2, 0, 0), vector(0.2673, 0.5345, 0.8018)))") {
            auto xs = c->local_intersect(ray(point(-2, 0, 0), vector(0.2673, 0.5345, 0.8018)));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(0, -2, 0), vector(0.8018, 0.2673, 0.5345)))") {
            auto xs = c->local_intersect(ray(point(0, -2, 0), vector(0.8018, 0.2673, 0.5345)));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(0, 0, -2), vector(0.5345, 0.8018, 0.2673)))") {
            auto xs = c->local_intersect(ray(point(0, 0, -2), vector(0.5345, 0.8018, 0.2673)));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(2, 0, 2), vector(0, 0, -1)))") {
            auto xs = c->local_intersect(ray(point(2, 0, 2), vector(0, 0, -1)));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(0, 2, 2), vector(0, -1, 0)))") {
            auto xs = c->local_intersect(ray(point(0, 2, 2), vector(0, -1, 0)));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(c, ray(point(2, 2, 0), vector(-1, 0, 0)))") {
            auto xs = c->local_intersect(ray(point(2, 2, 0), vector(-1, 0, 0)));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }
    }
}


SCENARIO("The normal on the surface of a cube.", "[cubes]") {
    GIVEN("c <- cube()") {
        auto c = cube();

        WHEN("normal <- local_normal_at(c, point(1, 0.5, -0.8))") {
            auto normal = c->local_normal_at(point(1, 0.5, -0.8));
            THEN("normal = vector(1, 0, 0)") {
                REQUIRE(normal == vector(1, 0, 0));
            }
        }

        WHEN("normal <- local_normal_at(c, point(-1, -0.2, 0.9))") {
            auto normal = c->local_normal_at(point(-1, -0.2, -0.9));
            THEN("normal = vector(-1, 0, 0)") {
                REQUIRE(normal == vector(-1, 0, 0));
            }
        }

        WHEN("normal <- local_normal_at(c, point(-0.4, 1, -0.1))") {
            auto normal = c->local_normal_at(point(-0.4, 1, -0.1));
            THEN("normal = vector(0, 1, 0)") {
                REQUIRE(normal == vector(0, 1, 0));
            }
        }

        WHEN("normal <- local_normal_at(c, point(0.3, -1, -0.7))") {
            auto normal = c->local_normal_at(point(0.3, -1, -0.7));
            THEN("normal = vector(0, -1, 0)") {
                REQUIRE(normal == vector(0, -1, 0));
            }
        }

        WHEN("normal <- local_normal_at(c, point(-0.6, 0.3, 1))") {
            auto normal = c->local_normal_at(point(-0.6, 0.3, 1));
            THEN("normal = vector(0, 0, 1)") {
                REQUIRE(normal == vector(0, 0, 1));
            }
        }

        WHEN("normal <- local_normal_at(c, point(0.4, 0.4, -1))") {
            auto normal = c->local_normal_at(point(0.4, 0.4, -1));
            THEN("normal = vector(0, 0, -1)") {
                REQUIRE(normal == vector(0, 0, -1));
            }
        }

        WHEN("normal <- local_normal_at(c, point(1, 1, 1))") {
            auto normal = c->local_normal_at(point(1, 1, 1));
            THEN("normal = vector(1, 0, 0)") {
                REQUIRE(normal == vector(1, 0, 0));
            }
        }

        WHEN("normal <- local_normal_at(c, point(-1, -1, -1))") {
            auto normal = c->local_normal_at(point(-1, -1, -1));
            THEN("normal = vector(-1, 0, 0)") {
                REQUIRE(normal == vector(-1, 0, 0));
            }
        }
    }
}
