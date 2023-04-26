/**
 * @file tests/world_feature.cpp
 * @brief Tests for rendering world and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/shapes.hpp>
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


SCENARIO("There is no shadow when nothing is collinear with point and light.", "[world]")
{
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("p <- point(0, 10, 0)") {
            auto p = point(0, 10, 0);
            THEN("is_shadowed(w, p) is false") {
                REQUIRE_FALSE(is_shadowed(w, p));
            }
        }
    }
}


SCENARIO("There shadow when an object is between the point and the light.", "[world]")
{
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("p <- point(10, -10, 10)") {
            auto p = point(10, -10, 10);
            THEN("is_shadowed(w, p) is true") {
                REQUIRE(is_shadowed(w, p));
            }
        }
    }
}


SCENARIO("There is no shadow when an object is behind the light.", "[world]")
{
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("p <- point(-20, 20, -20)") {
            auto p = point(-20, 20, -20);
            THEN("is_shadowed(w, p) is false") {
                REQUIRE_FALSE(is_shadowed(w, p));
            }
        }
    }
}


SCENARIO("There is no shadow when an object is behind the point.", "[world]")
{
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("p <- point(-2, 2, -2)") {
            auto p = point(-2, 2, -2);
            THEN("is_shadowed(w, p) is false") {
                REQUIRE_FALSE(is_shadowed(w, p));
            }
        }
    }
}


SCENARIO("shade_hit() is geven an intersection in shadow.", "[world]")
{
    GIVEN("w <- world()") {
        auto w = world();
        AND_GIVEN("w.light = point_light(point(0, 0, -10), color(1, 1, 1))") {
            w->light = point_light(point(0, 0, -10), color(1, 1, 1));
            AND_GIVEN("s1 <- sphere()") {
                auto s1 = sphere();
                AND_GIVEN("s1 is added to w") {
                    w->objects.push_back(s1);
                    AND_GIVEN("s1 <- sphere() with transform translation(0, 0, 10)") {
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


SCENARIO("The reflected color for a reflective material.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("shape <- plane() with reflective and transformation.") {
            auto shape = plane();
            shape->material->reflective = 0.5;
            shape->transform = translation(0, -1, 0);
            AND_GIVEN("shape is added to w") {
                w->objects.push_back(shape);
                AND_GIVEN("r <- ray(point(0, 0, -3), vector(0, -√2/2, √2/2))") {
                    auto r = ray(point(0, 0, -3), vector(0, -M_SQRT2 / 2, M_SQRT2 / 2));
                    AND_GIVEN("i <- intersection(√2, shape)") {
                        auto i = intersection(M_SQRT2, shape);
                        WHEN("comps <- prepare_computations(i, r)") {
                            auto comps = prepare_computations(i, r);
                            AND_WHEN("color <- reflected_color(w, comps)") {
                                auto clr = reflected_color(w, comps);
                                THEN("color = color(0.19033, 0.23792, 0.14275)") {
                                    REQUIRE(clr == color(0.19033, 0.23792, 0.14275));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("shade_hit() with a reflective material.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("shape <- plane() with reflective and transformation.") {
            auto shape = plane();
            shape->material->reflective = 0.5;
            shape->transform = translation(0, -1, 0);
            AND_GIVEN("shape is added to w") {
                w->objects.push_back(shape);
                AND_GIVEN("r <- ray(point(0, 0, -3), vector(0, -√2/2, √2/2))") {
                    auto r = ray(point(0, 0, -3), vector(0, -M_SQRT2 / 2, M_SQRT2 / 2));
                    AND_GIVEN("i <- intersection(√2, shape)") {
                        auto i = intersection(M_SQRT2, shape);
                        WHEN("comps <- prepare_computations(i, r)") {
                            auto comps = prepare_computations(i, r);
                            AND_WHEN("color <- shade_hit(w, comps)") {
                                auto c = shade_hit(w, comps);
                                THEN("color = color(0.87677, 0.92436, 0.82918)") {
                                    REQUIRE(c == color(0.87676, 0.92434, 0.82918));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("color_at() with mutually reflective surfaces.", "[world]") {
    GIVEN("w <- world()") {
        auto w = world();
        AND_GIVEN("w.light <- point_light(position(0, 0, 0), color(1, 1, 1))") {
            w->light = point_light(point(0, 0, 0), color(1, 1, 1));
            AND_GIVEN("lower <- plane()...") {
                auto lower = plane();
                lower->material->reflective = 1;
                lower->transform = translation(0, -1, 0);
                AND_GIVEN("lower is added to w") {
                    w->objects.push_back(lower);
                    AND_GIVEN("upper <- plane()...") {
                        auto upper = plane();
                        upper->material->reflective = 1;
                        upper->transform = translation(0, 1, 0);
                        AND_GIVEN("upper is added to w") {
                            w->objects.push_back(upper);
                            AND_GIVEN("r <- ray(point(0, 0, 0), vector(0, 1, 0))") {
                                auto r = ray(point(0, 0, 0), vector(0, 1, 0));
                                THEN("color_at(w, r) should terminate successfully") {
                                    auto c = color_at(w, r);
                                    REQUIRE(true);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("The reflected color at the maximum recursive depth.", "[world]") {
    GIVEN("w <- default_world()") {
        auto w = default_world();
        AND_GIVEN("shape <- plane()...") {
            auto shape = plane();
            shape->material->reflective = 0.5;
            shape->transform = translation(0, -1, 0);
            AND_GIVEN("shape is added to w") {
                w->objects.push_back(shape);
                AND_GIVEN("r <- ray(point(0, 0, -3), vector(0, -√2/2, √2/2))") {
                    auto r = ray(point(0, 0, -1), vector(0, -M_SQRT2/2, M_SQRT2/2));
                    AND_GIVEN("i <- intersection(√2, shape)") {
                        auto i = intersection(M_SQRT2, shape);
                        WHEN("comps <- prepare_computations(i, r)") {
                            auto comps = prepare_computations(i, r);
                            AND_WHEN("color <- reflected_color(w, comps, 0)") {
                                auto c = reflected_color(w, comps, 0);
                                THEN("c = color(0, 0, 0)") {
                                    REQUIRE(c == color(0, 0, 0));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
