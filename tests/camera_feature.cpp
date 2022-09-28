/**
 * @file tests/world_feature.cpp
 * @brief Tests for camera and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <myrtchallenge/camera.hpp>
#include <myrtchallenge/transformations.hpp>

#include "catch_helpers.hpp"


SCENARIO("Constructing a camera.", "[camera]") {
    GIVEN("hsize <- 160") {
        auto hsize = 160;
        AND_GIVEN("vsize <- 120") {
            auto vsize = 120;
            AND_GIVEN("field_of_view <- π/2") {
                auto field_of_view = M_PI_2;
                WHEN("c <- camera(hsize, vsize, field_of_view)") {
                    auto c = camera(hsize, vsize, field_of_view);
                    THEN("c.hsize = 160") {
                        REQUIRE(c->hsize == 160);
                        AND_THEN("c.vsize = 120") {
                            REQUIRE(c->vsize == 120);
                            AND_THEN("c.field_of_view = π/2") {
                                REQUIRE(c->field_of_view == M_PI_2);
                                AND_THEN("c.transform = identity_matrix") {
                                    REQUIRE(c->transform == identity_matrix());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The pixel size for a horizontal canvas.", "[camera]") {
    GIVEN("c <- camera(200, 125, π/2)") {
        auto c = camera(200, 125, M_PI_2);
        THEN("c.pixel_size = 0.01") {
            REQUIRE(c->pixel_size == Catch::Approx(0.01));
        }
    }
}


SCENARIO("The pixel size for a vertical camera.", "[camera]") {
    GIVEN("c <- camera(125, 200, π/2)") {
        auto c = camera(125, 200, M_PI_2);
        THEN("c.pixel_size = 0.01") {
            REQUIRE(c->pixel_size == Catch::Approx(0.01));
        }
    }
}


SCENARIO("Constructing a ray through the center of the canvas.", "[camera]") {
    GIVEN("c <- camera(201, 101, π/2)") {
        auto c = camera(201, 101, M_PI_2);
        WHEN("r <- ray_for_pixel(c, 100, 50)") {
            auto r = ray_for_pixel(c, 100, 50);
            THEN("r.origin = point(0, 0, 0)") {
                REQUIRE(r.origin == point(0, 0, 0));
                AND_THEN("r.direction = vector(0, 0, -1)") {
                    REQUIRE(r.direction == vector(0, 0, -1));
                }
            }
        }
    }
}


SCENARIO("Constructing a ray through a corner of the canvas.", "[camera]") {
    GIVEN("c <- camera(201, 101, π/2)") {
        auto c = camera(201, 101, M_PI_2);
        WHEN("r <- ray_for_pixel(c, 0, 0)") {
            auto r = ray_for_pixel(c, 0, 0);
            THEN("r.origin = point(0, 0, 0)") {
                REQUIRE(r.origin == point(0, 0, 0));
                AND_THEN("r.direction = vector(0.66519, 0.33259, -0.66851)") {
                    REQUIRE(r.direction == vector(0.66519, 0.33259, -0.66851));
                }
            }
        }
    }
}


SCENARIO("Constructing a ray when the camera is transformed.", "[camera]") {
    GIVEN("c <- camera(201, 101, π/2)") {
        auto c = camera(201, 101, M_PI_2);
        WHEN("c.transform = rotation_y(π/4) * translation(0, -2, 5)") {
            c->transform = rotation_y(M_PI_4) * translation(0, -2, 5);
            AND_WHEN("r <- ray_for_pixel(c, 100, 50)") {
                auto r = ray_for_pixel(c, 100, 50);
                THEN("r.origin = point(0, 2, -5)") {
                    REQUIRE(r.origin == point(0, 2, -5));
                    AND_THEN("r.direction = vector(√2/2, 0, -√2/2)") {
                        REQUIRE(r.direction == vector(M_SQRT2 / 2, 0, -M_SQRT2 / 2));
                    }
                }
            }
        }
    }
}


SCENARIO("Rendering a world with a camera.", "[camera]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("c <- camera(11, 11, π/2)") {
            auto c = camera(11, 11, M_PI_2);
            AND_GIVEN("from <- point(0, 0, -5)") {
                auto from = point(0, 0, -5);
                AND_GIVEN("to <- point(0, 0, 0)") {
                    auto to = point(0, 0, 0);
                    AND_GIVEN("up <- vector(0, 1, 0)") {
                        auto up = vector(0, 1, 0);
                        AND_GIVEN("c.transform = view_transform(from, to, up)") {
                            c->transform = view_transform(from, to, up);
                            WHEN("image <- render(c, w)") {
                                auto image = render(c, w);
                                THEN("pixel_at(image, 5, 5) = color(0.38066, 0.47583, 0.28550)") {
                                    REQUIRE(pixel_at(image, 5, 5) == color(0.38066, 0.47583, 0.28550));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
