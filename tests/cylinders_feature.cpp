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
    GIVEN("cyl <- cylinder()") {
        auto cyl = cylinder();

        WHEN("xs <- local_intersect(cyl, ray(point(1, 0, 0), normalize(vector(0, 1, 0))))") {
            auto xs = cyl->local_intersect(ray(point(1, 0, 0), normalize(vector(0, 1, 0))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 0, 0), normalize(vector(0, 1, 0))))") {
            auto xs = cyl->local_intersect(ray(point(0, 0, 0), normalize(vector(0, 1, 0))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 0, -5), normalize(vector(1, 1, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, 0, -5), normalize(vector(1, 1, 1))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }
    }
}


SCENARIO("A ray strikes a cylinder.", "[cylinders]") {
    GIVEN("cyl <- cylinder()") {
        auto cyl = cylinder();

        WHEN("xs <- cyl.local_intersection(ray(point(1, 0, -5), normalize(vector(0, 0, 1))))") {
            auto xs = cyl->local_intersect(ray(point(1, 0, -5), normalize(vector(0, 0, 1))));
            THEN("x[0].t = 5 and x[1].t = 5") {
                REQUIRE(equal(xs[0].t, 5));
                REQUIRE(equal(xs[1].t, 5));
            }
        }

        WHEN("xs <- cyl.local_intersection(ray(point(0, 0, -5), normalize(vector(0, 0, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, 0, -5), normalize(vector(0, 0, 1))));
            THEN("x[0].t = 4 and x[1].t = 6") {
                REQUIRE(equal(xs[0].t, 4));
                REQUIRE(equal(xs[1].t, 6));
            }
        }

        WHEN("xs <- cyl.local_intersection(ray(point(0.5, 0, -5), normalize(vector(0.1, 1, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0.5, 0, -5), normalize(vector(0.1, 1, 1))));
            THEN("x[0].t = 6.80798 and x[1].t = 7.08872") {
                REQUIRE(equal(xs[0].t, 6.80798));
                REQUIRE(equal(xs[1].t, 7.08872));
            }
        }
    }
}


SCENARIO("Normal vector on a cylinder.", "[cylinders]") {
    GIVEN("cyl <- cylinder()") {
        auto cyl = cylinder();

        WHEN("normal <- local_normal_at(cyl, point(1, 0, 0))") {
            auto normal = cyl->local_normal_at(point(1, 0, 0));
            THEN("normal = vector(1, 0, 0)") {
                REQUIRE(normal == vector(1, 0, 0));
            }
        }

        WHEN("normal <- local_normal_at(cyl, point(0, 5, -1))") {
            auto normal = cyl->local_normal_at(point(0, 5, -1));
            THEN("normal = vector(0, 0, -1)") {
                REQUIRE(normal == vector(0, 0, -1));
            }
        }

        WHEN("normal <- local_normal_at(cyl, point(0, -2, 1))") {
            auto normal = cyl->local_normal_at(point(0, -2, 1));
            THEN("normal = vector(0, 0, 1)") {
                REQUIRE(normal == vector(0, 0, 1));
            }
        }

        WHEN("normal <- local_normal_at(cyl, point(-1, 1, 0))") {
            auto normal = cyl->local_normal_at(point(-1, 1, 0));
            THEN("normal = vector(-1, 0, 0)") {
                REQUIRE(normal == vector(-1, 0, 0));
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
    GIVEN("cyl <- cylinder()") {
        auto cyl = std::dynamic_pointer_cast<Cylinder>(cylinder());
        cyl->minimum = 1;
        cyl->maximum = 2;

        WHEN("xs <- local_intersect(cyl, ray(point(0, 1.5, 0), normalize(vector(0.1, 1, 0))))") {
            auto xs = cyl->local_intersect(ray(point(0, 1.5, 0), normalize(vector(0.1, 1, 0))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 3, -5), normalize(vector(0, 0, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, 3, -5), normalize(vector(0, 0, 1))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 0, -5), normalize(vector(0, 0, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, 0, -5), normalize(vector(0, 0, 1))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 2, -5), normalize(vector(0, 0, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, 2, -5), normalize(vector(0, 0, 1))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 1, -5), normalize(vector(0, 0, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, 1.5, 0), normalize(vector(0.1, 1, 0))));
            THEN("xs.count = 0") {
                REQUIRE(xs.size() == 0);
                REQUIRE(xs.empty());
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 1.5, -2), normalize(vector(0, 0, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, 1.5, -2), normalize(vector(0, 0, 1))));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
                REQUIRE(!xs.empty());
            }
        }

    }
}


SCENARIO("The default closed value for a cylinder.", "[cylinders]") {
    GIVEN("cyl <- cylinder()") {
        auto cyl = std::dynamic_pointer_cast<Cylinder>(cylinder());
        THEN("cyl.closed = false") {
            REQUIRE(!cyl->closed);
        }
    }
}


SCENARIO("Intersecting the caps of a closed cylinder.", "[cylinders]") {
    GIVEN("cyl <- cylinder()") {
        auto cyl = std::dynamic_pointer_cast<Cylinder>(cylinder());
        cyl->minimum = 1;
        cyl->maximum = 2;
        cyl->closed = true;

        WHEN("xs <- local_intersect(cyl, ray(point(0, 3, 0), normalize(vector(0, -1, 0))))") {
            auto xs = cyl->local_intersect(ray(point(0, 3, 0), normalize(vector(0, -1, 0))));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 3, -2), normalize(vector(0, -1, 2))))") {
            auto xs = cyl->local_intersect(ray(point(0, 3, -2), normalize(vector(0, -1, 2))));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 4, -2), normalize(vector(0, -1, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, 4, -2), normalize(vector(0, -1, 1))));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, 0, -2), normalize(vector(0, 1, 2))))") {
            auto xs = cyl->local_intersect(ray(point(0, 0, -2), normalize(vector(0, 1, 2))));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
        }

        WHEN("xs <- local_intersect(cyl, ray(point(0, -1, -2), normalize(vector(0, 1, 1))))") {
            auto xs = cyl->local_intersect(ray(point(0, -1, -2), normalize(vector(0, 1, 1))));
            THEN("xs.count = 2") {
                REQUIRE(xs.size() == 2);
            }
        }
    }
}


SCENARIO("Normal vector on a cylinder's end caps.", "[cylinders]") {
    GIVEN("cyl <- cylinder()") {
        auto cyl = std::dynamic_pointer_cast<Cylinder>(cylinder());
        cyl->minimum = 1;
        cyl->maximum = 2;
        cyl->closed = true;

        WHEN("normal = local_normal_at(cyl, point(0, 1, 0))") {
            auto normal = cyl->local_normal_at(point(0, 1, 0));
            THEN("normal = vector(0, -1, 0)") {
                REQUIRE(normal == vector(0, -1, 0));
            }
        }

        WHEN("normal = local_normal_at(cyl, point(0.5, 1, 0))") {
            auto normal = cyl->local_normal_at(point(0.5, 1, 0));
            THEN("normal = vector(0, -1, 0)") {
                REQUIRE(normal == vector(0, -1, 0));
            }
        }

        WHEN("normal = local_normal_at(cyl, point(0, 1, 0.5))") {
            auto normal = cyl->local_normal_at(point(0, 1, 0.5));
            THEN("normal = vector(0, -1, 0)") {
                REQUIRE(normal == vector(0, -1, 0));
            }
        }

        WHEN("normal = local_normal_at(cyl, point(0, 2, 0))") {
            auto normal = cyl->local_normal_at(point(0, 2, 0));
            THEN("normal = vector(0, 1, 0)") {
                REQUIRE(normal == vector(0, 1, 0));
            }
        }

        WHEN("normal = local_normal_at(cyl, point(0.5, 2, 0))") {
            auto normal = cyl->local_normal_at(point(0.5, 2, 0));
            THEN("normal = vector(0, 1, 0)") {
                REQUIRE(normal == vector(0, 1, 0));
            }
        }

        WHEN("normal = local_normal_at(cyl, point(0, 2, 0.5))") {
            auto normal = cyl->local_normal_at(point(0, 2, 0.5));
            THEN("normal = vector(0, 1, 0)") {
                REQUIRE(normal == vector(0, 1, 0));
            }
        }
    }
}
