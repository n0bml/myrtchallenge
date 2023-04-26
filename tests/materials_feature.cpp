/**
 * @file tests/materials_feature.cpp
 * @brief Tests for materials and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/materials.hpp>
#include <myrtchallenge/patterns.hpp>
#include <myrtchallenge/shapes.hpp>

#include "catch_helpers.hpp"


static decltype(auto) g_m = material();
static decltype(auto) g_position = point(0, 0, 0);


SCENARIO("The default material.", "[materials]")
{
    GIVEN("m <- material()") {
        auto m = material();
        THEN("m.color = color(1, 1, 1)") {
            REQUIRE(m->color == color(1, 1, 1));
            AND_THEN("m.ambient = 0.1") {
                REQUIRE(m->ambient == 0.1);
                AND_THEN("m.diffuse = 0.9") {
                    REQUIRE(m->diffuse == 0.9);
                    AND_THEN("m.specular = 0.9") {
                        REQUIRE(m->specular == 0.9);
                        AND_THEN("m.shininess = 200.0") {
                            REQUIRE(m->shininess == 200.0);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Lighting with the eye between the light and the surface.", "[materials]")
{
    GIVEN("eyev <- vector(0, 0, -1)") {
        auto eyev = vector(0, 0, -1);
        AND_GIVEN("normalv <- vector(0, 0, -1)") {
            auto normalv = vector(0, 0, -1);
            AND_GIVEN("light <- point_light(point(0, 0, -10), color(1, 1, 1))") {
                auto light = point_light(point(0, 0, -10), color(1, 1, 1));
                WHEN("result <- lighting(m, object, light, position, eyev, normalv, false)") {
                    auto object = sphere();
                    auto result = lighting(g_m, object, light, g_position, eyev, normalv, false);
                    THEN("result = color(1.9, 1.9, 1.9)") {
                        REQUIRE(result == color(1.9, 1.9, 1.9));
                    }
                }
            }
        }
    }
}


SCENARIO("Lighting with the eye between light and surface, eye offset 45°.", "[materials]")
{
    GIVEN("eyev <- vector(0, √2/2, -√2/2)") {
        auto eyev = vector(0, M_SQRT2 / 2, -M_SQRT2 / 2);
        AND_GIVEN("normalv <- vector(0, 0, -1)") {
            auto normalv = vector(0, 0, -1);
            AND_GIVEN("light <- point_light(point(0, 0, -10), color(1, 1, 1))") {
                auto light = point_light(point(0, 0, -10), color(1, 1, 1));
                WHEN("result <- lighting(m, object, light, position, eyev, normalv, false)") {
                    auto object = sphere();
                    auto result = lighting(g_m, object, light, g_position, eyev, normalv, false);
                    THEN("result = color(1.0, 1.0, 1.0)") {
                        REQUIRE(result == color(1.0, 1.0, 1.0));
                    }
                }
            }
        }
    }
}


SCENARIO("Lighting with eye opposite surface, light offset 45°.", "[materials]")
{
    GIVEN("eyev <- vector(0, 0, -1)") {
        auto eyev = vector(0, 0, -1);
        AND_GIVEN("normalv <- vector(0, 0, -1)") {
            auto normalv = vector(0, 0, -1);
            AND_GIVEN("light <- point_light(point(0, 10, -10), color(1, 1, 1)") {
                auto light = point_light(point(0, 10, -10), color(1, 1, 1));
                WHEN("result = lighting(m, object, light, position, eyev, normalv, false)") {
                    auto object = sphere();
                    auto result = lighting(g_m, object, light, g_position, eyev, normalv, false);
                    THEN("result = color(0.7364, 0.7364, 0.7364)") {
                        REQUIRE(result == color(0.7364, 0.7364, 0.7364));
                    }
                }
            }
        }
    }
}


SCENARIO("Lighting with eye in the path of the reflection vector.", "[materials]")
{
    GIVEN("eyev <- vector(0, -√2/2, -√2/2)") {
        auto eyev = vector(0, -M_SQRT2 / 2, -M_SQRT2 / 2);
        AND_GIVEN("normalv <- vector(0, 0, -1)") {
            auto normalv = vector(0, 0, -1);
            AND_GIVEN("light <- point_light(point(0, 10, -10), color(1, 1, 1)") {
                auto light = point_light(point(0, 10, -10), color(1, 1, 1));
                WHEN("result <- lighting(m, object, light, position, eyev, normalv, false)") {
                    auto object = sphere();
                    auto result = lighting(g_m, object, light, g_position, eyev, normalv, false);
                    THEN("result = color(1.6364, 1.6364, 1.6364)") {
                        REQUIRE(result == color(1.6364, 1.6364, 1.6364));
                    }
                }
            }
        }
    }
}



SCENARIO("Lighting with the light behind the surface.", "[materials]")
{
    GIVEN("eyev <- vector(0, 0, -1)") {
        auto eyev = vector(0, 0, -1);
        AND_GIVEN("normalv <- vector(0, 0, -1)") {
            auto normalv = vector(0, 0, -1);
            AND_GIVEN("light <- point_light(point(0, 0, 10), color(1, 1, 1)") {
                auto light = point_light(point(0, 0, 10), color(1, 1, 1));
                WHEN("result <- lighting(m, object, light, position, eyev, normalv, false)") {
                    auto object = sphere();
                    auto result = lighting(g_m, object, light, g_position, eyev, normalv, false);
                    THEN("result = color(0.1, 0.1, 0.1)") {
                        REQUIRE(result == color(0.1, 0.1, 0.1));
                    }
                }
            }
        }
    }
}


SCENARIO("Lighting with the surface in shadow.", "[materials]") {
    GIVEN("eyev <- vector(0, 0, -1)") {
        auto eyev = vector(0, 0, -1);
        AND_GIVEN("normalv <- vector(0, 0, -1)") {
            auto normalv = vector(0, 0, -1);
            AND_GIVEN("light <- point_light(point(0, 0, -10), color(1, 1, 1))") {
                auto light = point_light(point(0, 0, -10), color(1, 1, 1));
                AND_GIVEN("in_shadow <- true") {
                    auto in_shadow = true;
                    WHEN("result <- lighting(m, object, light, position, eyev, normalv, in_shadow)") {
                        auto object = sphere();
                        auto result = lighting(g_m, object, light, g_position, eyev, normalv, in_shadow);
                        THEN("result = color(0.1, 0.1, 0.1)") {
                            REQUIRE(result == color(0.1, 0.1, 0.1));
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Lighting with a pattern applied.", "[materials]") {
    GIVEN("m.pattern <- stripe_pattern(color(1, 1, 1), color(0, 0, 0))") {
        g_m->pattern = stripe_pattern(color(1, 1, 1), color(0, 0, 0));
        AND_GIVEN("m.ambient <- 1") {
            g_m->ambient = 1;
            AND_GIVEN("m.diffuse <- 0") {
                g_m->diffuse = 0;
                AND_GIVEN("m.specular <- 0") {
                    g_m->specular = 0;
                    AND_GIVEN("eyev <- vector(0, 0, -1)") {
                        auto eyev = vector(0, 0, -1);
                        AND_GIVEN("normalv <- vector(0, 0, -1)") {
                            auto normalv = vector(0, 0, -1);
                            AND_GIVEN("light <- point_light(point(0, 0, -10), color(1, 1, 1))") {
                                auto light = point_light(point(0, 0, -10), color(1, 1, 1));
                                WHEN("c1 <- lighting(m, object, light, point(0.9, 0, 0), eyev, normalv, false)") {
                                    auto object = sphere();
                                    auto c1 = lighting(g_m, object, light, point(0.9, 0, 0), eyev, normalv, false);
                                    AND_WHEN("c2 <- lighting(m, object, light, point(1.1, 0, 0), eyev, normalv, false)") {
                                        auto c2 = lighting(g_m, object, light, point(1.1, 0, 0), eyev, normalv, false);
                                        THEN("c1 = color(1, 1, 1)") {
                                            REQUIRE(c1 == color(1, 1, 1));
                                            AND_THEN("c2 = color(0, 0, 0)") {
                                                REQUIRE(c2 == color(0, 0, 0));
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


SCENARIO("Reflectivity for the default material.", "[materials]") {
    GIVEN("m <- material()") {
        auto m = material();
        THEN("m.reflective = 0.0") {
            REQUIRE(m->reflective == 0.0);
        }
    }
}


SCENARIO("Transparency and Refractive Index for the default material.", "[materials]") {
    GIVEN("m <- material()") {
        auto m = material();
        THEN("m.transparency = 0.0") {
            REQUIRE(m->transparency == 0.0);
            AND_THEN("m.refractive_index = 1.0") {
                REQUIRE(m->refractive_index == 1.0);
            }
        }
    }
}
