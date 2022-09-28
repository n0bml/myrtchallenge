/**
 * @file tests/world_feature.cpp
 * @brief Tests for rendering world and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/transformations.hpp>
#include <myrtchallenge/world.hpp>

#include "catch_helpers.hpp"


SCENARIO("Creating a world.", "[world]")
{
    GIVEN("w <- world()") {
        auto w = world();
        THEN("w contains no objects.") {
            REQUIRE(w->objects.empty());
            AND_THEN("w has no light source.") {
                REQUIRE(w->light == nullptr);
            }
        }
    }
}


SCENARIO("The default world.", "[world]") {
    GIVEN("light <- point_light(point(-10, 10, -10), color(1, 1, 1))") {
        auto light = point_light(point(-10, 10, -10), color(1, 1, 1));
        AND_GIVEN("s1 <- sphere() with specified material.") {
            auto s1 = sphere();
            s1->material->color    = color(0.8, 1.0, 0.6);
            s1->material->diffuse  = 0.7;
            s1->material->specular = 0.2;
            AND_GIVEN("s2 <- sphere() with specified transform.") {
                auto s2 = sphere();
                s2->transform = scaling(0.5, 0.5, 0.5);
                WHEN("w <- default_world()") {
                    auto w = default_world();
                    THEN("w.light = light") {
                        REQUIRE(*w->light == *light);
                        AND_THEN("w contains s1") {
                            REQUIRE(*w->objects[0] == *s1);
                            AND_THEN("w contains s2") {
                                REQUIRE(*w->objects[1] == *s2);
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Intersect a world with a ray.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
            auto r = ray(point(0, 0, -5), vector(0, 0, 1));
            WHEN("xs <- intersect_world(w, r)") {
                auto xs = intersect_world(w, r);
                THEN("xs.count = 4") {
                    REQUIRE(xs.size() == 4);
                    AND_THEN("xs[0].t = 4") {
                        REQUIRE(xs[0].t == 4);
                        AND_THEN("xs[1].t = 4.5") {
                            REQUIRE(xs[1].t == 4.5);
                            AND_THEN("xs[2].t = 5.5") {
                                REQUIRE(xs[2].t == 5.5);
                                AND_THEN("xs[3].t = 6") {
                                    REQUIRE(xs[3].t == 6);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Shading an intersection.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
            auto r = ray(point(0, 0, -5), vector(0, 0, 1));
            AND_GIVEN("shape <- the first object in w") {
                auto shape = w->objects[0];
                AND_GIVEN("i <- intersection(4, shape)") {
                    auto i = intersection(4, shape);
                    WHEN("comps <- prepare_computations(i, r)") {
                        auto comps = prepare_computations(i, r);
                        AND_WHEN("c <- shade_hit(w, comps)") {
                            auto c = shade_hit(w, comps);
                            THEN("c = color(0.38066, 0.47583, 0.28550)") {
                                REQUIRE(c == color(0.38066, 0.47583, 0.28550));
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Shading an intersection from the inside.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("w.light <- point_light(point(0, 0.25, 0), color(1, 1, 1))") {
            w->light = point_light(point(0, 0.25, 0), color(1, 1, 1));
            AND_GIVEN("r <- ray(point(0, 0, 0), vector(0, 0, 1))") {
                auto r = ray(point(0, 0, 0), vector(0, 0, 1));
                AND_GIVEN("shape <- the second object in w") {
                    auto shape = w->objects[1];
                    AND_GIVEN("i <- intersection(0.5, shape)") {
                        auto i = intersection(0.5, shape);
                        WHEN("comps <- prepare_computations(i, r)") {
                            auto comps = prepare_computations(i, r);
                            AND_WHEN("c <- shade_hit(w, comps)") {
                                auto c = shade_hit(w, comps);
                                THEN("c = color(0.90498, 0.90498, 0.90498)") {
                                    REQUIRE(c == color(0.90498, 0.90498, 0.90498));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The color when a ray misses.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("r <- ray(point(0, 0, -5), vector(0, 1, 0))") {
            auto r = ray(point(0, 0, -5), vector(0, 1, 0));
            WHEN("c <- color_at(w, r)") {
                auto c = color_at(w, r);
                THEN("c = color(0, 0, 0)") {
                    REQUIRE(c == color(0, 0, 0));
                }
            }
        }
    }
}


SCENARIO("The color when a ray hits.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("r <- ray(point(0, 0, -5), vector(0, 0, 1))") {
            auto r = ray(point(0, 0, -5), vector(0, 0, 1));
            WHEN("c <- color_at(w, r)") {
                auto c = color_at(w, r);
                THEN("c = color(0.38066, 0.47583, 0.28550)") {
                    REQUIRE(c == color(0.38066, 0.47583, 0.28550));
                }
            }
        }
    }
}


SCENARIO("The color with an intersection behind the ray.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("outer <- the first object in w") {
            auto outer = w->objects[0];
            AND_GIVEN("inner <- the second object in w") {
                auto inner = w->objects[1];
                AND_GIVEN("inner.material.ambient = 1") {
                    inner->material->ambient = 1;
                    AND_GIVEN("r <- ray(point(0, 0, 0.75), vector(0, 0, -1))") {
                        auto r = ray(point(0, 0, 0.75), vector(0, 0, -1));
                        WHEN("c <- color_at(w, r)") {
                            auto c = color_at(w, r);
                            THEN("c = inner.material.color") {
                                REQUIRE(c == inner->material->color);
                            }
                        }
                    }
                }
            }
        }
    }
}
