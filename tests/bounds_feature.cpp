/**
 * @file tests/bounds_feature.cpp
 * @brief Tests for bounding boxes and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022-23 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>

#include "catch_helpers.hpp"
#include "test_shape.hpp"


SCENARIO("Creating an empty bounding box.", "[bounds]") {
    GIVEN("box <- Bounds{}") {
        auto box = Bounds{};
    THEN("box.lower = point(infinity, infinity, infinity)") {
        REQUIRE(box.lower == point(INFINITY, INFINITY, INFINITY));
        AND_THEN("box.upper = point(-infinity, -infinity, -infinity)") {
            REQUIRE(box.upper == point(-INFINITY, -INFINITY, -INFINITY));
    }}}
}


SCENARIO("Creating a bounding box with volume.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(-1, -2, -3) max=point(3, 2, 1))") {
        Bounds box{point(-1, -2, -3), point(3, 2, 1)};
    THEN("box.min = point(-1, -2, -3)") {
        REQUIRE(box.lower == point(-1, -2, -3));
        AND_THEN("box.max = point(3, 2, 1)") {
            REQUIRE(box.upper == point(3, 2, 1));
    }}}
}


SCENARIO("A sphere has a bounding box.", "[bounds]") {
    GIVEN("shape ← sphere()") {
        auto shape = sphere();
    WHEN("box ← bounds_of(shape)") {
        auto box = shape->local_bounds();
    THEN("box.min = point(-1, -1, -1)") {
        REQUIRE(box.lower == point(-1, -1, -1));
        AND_THEN("box.max = point(1, 1, 1)") {
            REQUIRE(box.upper == point(1, 1, 1));
    }}}}
}


SCENARIO("A plane has a bounding box.", "[bounds]") {
    GIVEN("shape ← plane()") {
        auto shape = plane();
    WHEN("box ← bounds_of(shape)") {
        auto box = shape->local_bounds();
    THEN("box.min = point(-infinity, 0, -infinity)") {
        REQUIRE(box.lower == point(-INFINITY, 0, -INFINITY));
        AND_THEN("box.max = point(infinity, 0, infinity)") {
            REQUIRE(box.upper == point(INFINITY, 0, INFINITY));
    }}}}
}


SCENARIO("A cube has a bounding box.", "[bounds]") {
    GIVEN("shape ← cube()") {
        auto shape = cube();
    WHEN("box ← bounds_of(shape)") {
        auto box = shape->local_bounds();
    THEN("box.min = point(-1, -1, -1)") {
        REQUIRE(box.lower == point(-1, -1, -1));
        AND_THEN("box.max = point(1, 1, 1)") {
            REQUIRE(box.upper == point(1, 1, 1));
    }}}}
}


SCENARIO("An unbounded cylinder has a bounding box.", "[bounds]") {
    GIVEN("shape ← cylinder()") {
        auto shape = cylinder();
    WHEN("box ← bounds_of(shape)") {
        auto box = shape->local_bounds();
    THEN("box.min = point(-1, -infinity, -1)") {
        REQUIRE(box.lower == point(-1, -INFINITY, -1));
        AND_THEN("box.max = point(1, infinity, 1)") {
            REQUIRE(box.upper == point(1, INFINITY, 1));
    }}}}
}
    

SCENARIO("A bounded cylinder has a bounding box.", "[bounds]") {
    GIVEN("shape ← cylinder()") {
        auto shape = cylinder();
        AND_GIVEN("shape.minimum ← -5") {
            shape->minimum = -5;
        AND_GIVEN("shape.maximum ← 3") {
            shape->maximum = 3;
    WHEN("box ← bounds_of(shape)") {
        auto box = shape->local_bounds();
    THEN("box.min = point(-1, -5, -1)") {
        REQUIRE(box.lower == point(-1, -5, -1));
        AND_THEN("box.max = point(1, 3, 1)") {
            REQUIRE(box.upper == point(1, 3, 1));
    }}}}}}
}


SCENARIO("An unbounded cone has a bounding box.", "[bounds]") {
    GIVEN("shape ← cone()") {
        auto shape = cone();
    WHEN("box ← bounds_of(shape)") {
        auto box = shape->local_bounds();
    THEN("box.min = point(-infinity, -infinity, -infinity)") {
        REQUIRE(box.lower == point(-INFINITY, -INFINITY, -INFINITY));
        AND_THEN("box.max = point(infinity, infinity, infinity)") {
            REQUIRE(box.upper == point(INFINITY, INFINITY, INFINITY));
    }}}}
}


SCENARIO("A bounded cone has a bounding box.", "[bounds]") {
    GIVEN("shape ← cone()") {
        auto shape = cone();
        AND_GIVEN("shape.minimum ← -5") {
            shape->minimum = -5;
        AND_GIVEN("shape.maximum ← 3") {
            shape->maximum = 3;
    WHEN("box ← bounds_of(shape)") {
        auto box = shape->local_bounds();
    THEN("box.min = point(-5, -5, -5)") {
        REQUIRE(box.lower == point(-5, -5, -5));
        AND_THEN("box.max = point(5, 3, 5)");
            REQUIRE(box.upper == point(5, 3, 5));
    }}}}}
}


// TODO
// Scenario: A triangle has a bounding box
// Given p1 ← point(-3, 7, 2)
//   And p2 ← point(6, 2, -4)
//   And p3 ← point(2, -1, -1)
//   And shape ← triangle(p1, p2, p3)
// When box ← bounds_of(shape)
// Then box.min = point(-3, -1, -4)
//   And box.max = point(6, 7, 2)


SCENARIO("Test shape has (arbitrary) bounds.", "[bounds]") {
    GIVEN("shape ← test_shape()") {
        Test_Shape shape;
    WHEN("box ← bounds_of(shape)") {
        auto box = shape.local_bounds();
    THEN("box.min = point(-1, -1, -1)") {
        REQUIRE(box.lower == point(-1, -1, -1));
        AND_THEN("box.max = point(1, 1, 1)") {
            REQUIRE(box.upper == point(1, 1, 1));
    }}}}
}


SCENARIO("Adding points to an empty bounding box.", "[bounds]") {
    GIVEN("box ← bounds()") {
        auto box = bounds();
        AND_GIVEN("p1 ← point(-5, 2, 0)") {
            auto p1 = point(-5, 2, 0);
        AND_GIVEN("p2 ← point(7, 0, -3)") {
            auto p2 = point(7, 0, -3);
    WHEN("p1 is added to box") {
        box.add(p1);
        AND_WHEN("p2 is added to box") {
            box.add(p2);
    THEN("box.min = point(-5, 0, -3)") {
        REQUIRE(box.lower == point(-5, 0, -3));
        AND_THEN("box.max = point(7, 2, 0)") {
            REQUIRE(box.upper == point(7, 2, 0));
    }}}}}}}
}


SCENARIO("Adding one bounding box to another.", "[bounds]") {
    GIVEN("box1 ← bounding_box(min=point(-5, -2, 0) max=point(7, 4, 4))") {
        auto box1 = Bounds{point(-5, -2, 0), point(7, 4, 4)};
        AND_GIVEN("box2 ← bounding_box(min=point(8, -7, -2) max=point(14, 2, 8))") {
            auto box2 = Bounds{point(8, -7, -2), point(14, 2, 8)};
    WHEN("box2 is added to box1") {
        box1.add(box2);
    THEN("box1.min = point(-5, -7, -2)") {
        REQUIRE(box1.lower == point(-5, -7, -2));
        AND_THEN("box1.max = point(14, 4, 8)") {
            REQUIRE(box1.upper == point(14, 4, 8));
    }}}}}
}


SCENARIO("Checking to see if a box contains a given point.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(5, -2, 0) max=point(11, 4, 7))") {
        auto box = Bounds{point(5, -2, 0), point(11, 4, 7)};
    // And p ← <point>
    THEN("box_contains_point(box, p) is <result>") {
        // Examples:
        //   | point           | result |
        //   | point(5, -2, 0) | true   |
        REQUIRE(box.contains(point(5, -2, 0)) == true);

        //   | point(11, 4, 7) | true   |
        REQUIRE(box.contains(point(11, 4, 7)) == true);

        //   | point(8, 1, 3)  | true   |
        REQUIRE(box.contains(point(8, 1, 3)) == true);

        //   | point(3, 0, 3)  | false  |
        REQUIRE(box.contains(point(3, 0, 3)) == false);

        //   | point(8, -4, 3) | false  |
        REQUIRE(box.contains(point(8, -4, 3)) == false);

        //   | point(8, 1, -1) | false  |
        REQUIRE(box.contains(point(8, 1, -1)) == false);

        //   | point(13, 1, 3) | false  |
        REQUIRE(box.contains(point(13, 1, 3)) == false);

        //   | point(8, 5, 3)  | false  |
        REQUIRE(box.contains(point(8, 5, 3)) == false);

        //   | point(8, 1, 8)  | false  |
        REQUIRE(box.contains(point(8, 1, 8)) == false);
    }}
}


SCENARIO("Checking to see if a box contains a given box.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(5, -2, 0) max=point(11, 4, 7))") {
        auto box = Bounds{point(5, -2, 0), point(11, 4, 7)};
        AND_GIVEN("box2 ← bounding_box(min=<min> max=<max>)") {
            Bounds box2;
    THEN("box_contains_box(box, box2) is <result>") {
        // Examples:
        //   | min              | max             | result |
        //   | point(5, -2, 0)  | point(11, 4, 7) | true   |
        REQUIRE(box.contains(Bounds{point(5, -2, 0), point(11, 4, 7)}) == true);

        //   | point(6, -1, 1)  | point(10, 3, 6) | true   |
        REQUIRE(box.contains(Bounds{point(6, -1, 1), point(10, 3, 6)}) == true);

        //   | point(4, -3, -1) | point(10, 3, 6) | false  |
        REQUIRE(box.contains(Bounds{point(4, -3, -1), point(10, 3, 6)}) == false);

        //   | point(6, -1, 1)  | point(12, 5, 8) | false  |
        REQUIRE(box.contains(Bounds{point(6, -1, 1), point(12, 5, 8)}) == false);
    }}}
}


SCENARIO("Transforming a bounding box.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(-1, -1, -1) max=point(1, 1, 1))") {
        auto box = Bounds{point(-1, -1, -1), point(1, 1, 1)};
        AND_GIVEN("matrix ← rotation_x(π / 4) * rotation_y(π / 4)") {
            auto matrix = rotation_x(M_PI_4) * rotation_y(M_PI_4);
    WHEN("box2 ← transform(box, matrix)") {
        auto box2 = box.transform(matrix);
    THEN("box2.min = point(-1.4142, -1.7071, -1.7071)") {
        REQUIRE(box2.lower == point(-1.41421, -1.7071, -1.70711));
        AND_THEN("box2.max = point(1.4142, 1.7071, 1.7071)") {
            REQUIRE(box2.upper == point(1.41421, 1.70711, 1.70711));
    }}}}}
}


SCENARIO("A group has a bounding box that contains its children.", "[bounds]") {
    GIVEN("s ← sphere()") {
        auto s = sphere();
        AND_GIVEN("set_transform(s, translation(2, 5, -3) * scaling(2, 2, 2))") {
            set_transform(s, translation(2, 5, -3) * scaling(2, 2, 2));
        AND_GIVEN("c ← cylinder()") {
            auto c = cylinder();
        AND_GIVEN("c.minimum ← -2") {
            c->minimum = -2;
        AND_GIVEN("c.maximum ← 2") {
            c->maximum = 2;
        AND_GIVEN("set_transform(c, translation(-4, -1, 4) * scaling(0.5, 1, 0.5))") {
            set_transform(c, translation(-4, -1, 4) * scaling(0.5, 1, 0.5));
        AND_GIVEN("shape ← group()") {
            auto shape = group();
        AND_GIVEN("add_child(shape, s)") {
            add_child(shape, s);
        AND_GIVEN("add_child(shape, c)") {
            add_child(shape, c);
    WHEN("box ← bounds_of(shape)") {
        auto box = bounds(shape);
    THEN("box.min = point(-4.5, -3, -5)") {
        REQUIRE(box.lower == point(-4.5, -3, -5));
        AND_THEN("box.max = point(4, 7, 4.5)") {
            REQUIRE(box.upper == point(4, 7, 4.5));
    }}}}}}}}}}}}
}


SCENARIO("Intersecting a ray with a bounding box at the origin.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(-1, -1, -1) max=point(1, 1, 1))") {
        auto box = Bounds{point(-1, -1, -1), point(1, 1, 1)};
        // And direction ← normalize(<direction>)
        // And r ← ray(<origin>, direction)
    THEN("intersects(box, r) is <result>") {
        // Examples:
        //   | origin            | direction        | result |
        //   | point(5, 0.5, 0)  | vector(-1, 0, 0) | true   |
        REQUIRE(box.intersects(ray(point(5, 0.5, 0), vector(-1, 0, 0))) == true);

        //   | point(-5, 0.5, 0) | vector(1, 0, 0)  | true   |
        REQUIRE(box.intersects(ray(point(-5, 0.5, 0), vector(1, 0, 0))) == true);

        //   | point(0.5, 5, 0)  | vector(0, -1, 0) | true   |
        REQUIRE(box.intersects(ray(point(0.5, 5, 0), vector(0, -1, 0))) == true);

        //   | point(0.5, -5, 0) | vector(0, 1, 0)  | true   |
        REQUIRE(box.intersects(ray(point(0.5, -5, 0), vector(0, 1, 0))) == true);

        //   | point(0.5, 0, 5)  | vector(0, 0, -1) | true   |
        REQUIRE(box.intersects(ray(point(0.5, 0, 5), vector(0, 0, -1))) == true);

        //   | point(0.5, 0, -5) | vector(0, 0, 1)  | true   |
        REQUIRE(box.intersects(ray(point(0.5, 0, -5), vector(0, 0, 1))) == true);

        //   | point(0, 0.5, 0)  | vector(0, 0, 1)  | true   |
        REQUIRE(box.intersects(ray(point(0, 0.5, 0), vector(0, 0, 1))) == true);

        //   | point(-2, 0, 0)   | vector(2, 4, 6)  | false  |
        REQUIRE(box.intersects(ray(point(-2, 0, 0), vector(2, 4, 6))) == false);

        //   | point(0, -2, 0)   | vector(6, 2, 4)  | false  |
        REQUIRE(box.intersects(ray(point(0, -2, 0), vector(6, 2, 4))) == false);

        //   | point(0, 0, -2)   | vector(4, 6, 2)  | false  |
        REQUIRE(box.intersects(ray(point(0, 0, -2), vector(4, 6, 2))) == false);

        //   | point(2, 0, 2)    | vector(0, 0, -1) | false  |
        REQUIRE(box.intersects(ray(point(2, 0, 2), vector(0, 0, -1))) == false);

        //   | point(0, 2, 2)    | vector(0, -1, 0) | false  |
        REQUIRE(box.intersects(ray(point(0, 2, 2), vector(0, -1, 0))) == false);

        //   | point(2, 2, 0)    | vector(-1, 0, 0) | false  |
        REQUIRE(box.intersects(ray(point(2, 2, 0), vector(-1, 0, 0))) == false);
    }}
}


SCENARIO("Intersecting a ray with a non-cubic bounding box.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(5, -2, 0) max=point(11, 4, 7))") {
        auto box = Bounds{point(5, -2, 0), point(11, 4, 7)};
        // And direction ← normalize(<direction>)
        // And r ← ray(<origin>, direction)
    THEN("intersects(box, r) is <result>") {
        // Examples:
        //   | origin           | direction        | result |
        //   | point(15, 1, 2)  | vector(-1, 0, 0) | true   |
        REQUIRE(box.intersects(ray(point(15, 1, 2), normalize(vector(-1, 0, 0)))) == true);

        //   | point(-5, -1, 4) | vector(1, 0, 0)  | true   |
        REQUIRE(box.intersects(ray(point(-5, -1, 4), normalize(vector(1, 0, 0)))) == true);

        //   | point(7, 6, 5)   | vector(0, -1, 0) | true   |
        REQUIRE(box.intersects(ray(point(6, 6, 5), normalize(vector(0, -1, 0)))) == true);

        //   | point(9, -5, 6)  | vector(0, 1, 0)  | true   |
        REQUIRE(box.intersects(ray(point(9, -5, 6), normalize(vector(0, 1, 0)))) == true);

        //   | point(8, 2, 12)  | vector(0, 0, -1) | true   |
        REQUIRE(box.intersects(ray(point(8, 2, 12), normalize(vector(0, 0, -1)))) == true);

        //   | point(6, 0, -5)  | vector(0, 0, 1)  | true   |
        REQUIRE(box.intersects(ray(point(6, 0, -5), normalize(vector(0, 0, 1)))) == true);

        //   | point(8, 1, 3.5) | vector(0, 0, 1)  | true   |
        REQUIRE(box.intersects(ray(point(8, 1, 3.5), normalize(vector(0, 0, 1)))) == true);

        //   | point(9, -1, -8) | vector(2, 4, 6)  | false  |
        REQUIRE(box.intersects(ray(point(9, -1, -8), normalize(vector(2, 4, 6)))) == false);

        //   | point(8, 3, -4)  | vector(6, 2, 4)  | false  |
        REQUIRE(box.intersects(ray(point(8, 3, -4), normalize(vector(6, 2, 4)))) == false);

        //   | point(9, -1, -2) | vector(4, 6, 2)  | false  |
        REQUIRE(box.intersects(ray(point(9, -1, -2), normalize(vector(4, 6, 2)))) == false);

        //   | point(4, 0, 9)   | vector(0, 0, -1) | false  |
        REQUIRE(box.intersects(ray(point(4, 0, 9), normalize(vector(0, 0, -1)))) == false);

        //   | point(8, 6, -1)  | vector(0, -1, 0) | false  |
        REQUIRE(box.intersects(ray(point(8, 6, -1), normalize(vector(0, -1, 0)))) == false);

        //   | point(12, 5, 4)  | vector(-1, 0, 0) | false  |
        REQUIRE(box.intersects(ray(point(12, 5, 4), normalize(vector(-1, 0, 0)))) == false);
    }}
}


SCENARIO("Splitting a perfect cube.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(-1, -4, -5) max=point(9, 6, 5))") {
        auto box = Bounds{point(-1, -4, -5), point(9, 6, 5)};
    WHEN("(left, right) ← split_bounds(box)") {
        auto [left, right] = box.split();
    THEN("left.min = point(-1, -4, -5)") {
        REQUIRE(left.lower == point(-1, -4, -5));
        AND_THEN("left.max = point(4, 6, 5)") {
            REQUIRE(left.upper == point(4, 6, 5));
        AND_THEN("right.min = point(4, -4, -5)") {
            REQUIRE(right.lower == point(4, -4, -5));
        AND_THEN("right.max = point(9, 6, 5)") {
            REQUIRE(right.upper == point(9, 6, 5));
    }}}}}}
}


SCENARIO("Splitting an x-wide box.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(-1, -2, -3) max=point(9, 5.5, 3))") {
        auto box = Bounds{point(-1, -2, -3), point(9, 5.5, 3)};
    WHEN("(left, right) ← split_bounds(box)") {
        auto [left, right] = box.split();
    THEN("left.min = point(-1, -2, -3)") {
        REQUIRE(left.lower == point(-1, -2, -3));
        AND_THEN("left.max = point(4, 5.5, 3)") {
            REQUIRE(left.upper == point(4, 5.5, 3));
        AND_THEN("right.min = point(4, -2, -3)") {
            REQUIRE(right.lower == point(4, -2, -3));
        AND_THEN("right.max = point(9, 5.5, 3)") {
            REQUIRE(right.upper == point(9, 5.5, 3));
    }}}}}}
}


SCENARIO("Splitting a y-wide box.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(-1, -2, -3) max=point(5, 8, 3))") {
        auto box = Bounds{point(-1, -2, -3), point(5, 8, 3)};
    WHEN("(left, right) ← split_bounds(box)") {
        auto [left, right] = box.split();
    THEN("left.min = point(-1, -2, -3)") {
        REQUIRE(left.lower == point(-1, -2, -3));
        AND_THEN("left.max = point(5, 3, 3)") {
            REQUIRE(left.upper == point(5, 3, 3));
        AND_THEN("right.min = point(-1, 3, -3)") {
            REQUIRE(right.lower == point(-1, 3, -3));
        AND_THEN("right.max = point(5, 8, 3)") {
            REQUIRE(right.upper == point(5, 8, 3));
    }}}}}}
}


SCENARIO("Splitting a z-wide box.", "[bounds]") {
    GIVEN("box ← bounding_box(min=point(-1, -2, -3) max=point(5, 3, 7))") {
        auto box = Bounds{point(-1, -2, -3), point(5, 3, 7)};
    WHEN("(left, right) ← split_bounds(box)") {
        auto [left, right] = box.split();
    THEN("left.min = point(-1, -2, -3)") {
        REQUIRE(left.lower == point(-1, -2, -3));
        AND_THEN("left.max = point(5, 3, 2)") {
            REQUIRE(left.upper == point(5, 3, 2));
        AND_THEN("right.min = point(-1, -2, 2)") {
            REQUIRE(right.lower == point(-1, -2, 2));
        AND_THEN("right.max = point(5, 3, 7)") {
            REQUIRE(right.upper == point(5, 3, 7));
    }}}}}}
}
