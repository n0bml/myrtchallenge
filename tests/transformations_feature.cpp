/**
 * @file tests/transformations_feature.cpp
 * @brief Tests for matrix transformations and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/transformations.hpp>
#include <myrtchallenge/tuples.hpp>

#include "catch_helpers.hpp"


SCENARIO("Multiplying by a translation matrix.", "[transformations]")
{
    GIVEN("transform <- translation(5, -3, 2)") {
        auto transform = translation(5, -3, 2);
        AND_GIVEN("p <- point(-3, 4, 5)") {
            auto p = point(-3, 4, 5);
            THEN("transform * p = point(2, 7, 1)") {
                REQUIRE(transform * p == point(2, 1, 7));
            }
        }
    }
}


SCENARIO("Multiplying by the inverse of a translation matrix.", "[transformations]")
{
    GIVEN("transform <- translation(5, -3, 2)") {
        auto transform = translation(5, -3, 2);
        AND_GIVEN("inv <- inverse(transform)") {
            auto inv = inverse(transform);
            AND_GIVEN("p <- point(-3, 4, 5)") {
                auto p = point(-3, 4, 5);
                THEN("inv * p = point(-8, 7, 3)") {
                    REQUIRE(inv * p == point(-8, 7, 3));
                }
            }
        }
    }
}


SCENARIO("Translation does not affect vectors.", "[transformations]")
{
    GIVEN("transform <- translation(5, -3, 2)") {
        auto transform = translation(5, -3, 2);
        AND_GIVEN("v <- vector(-3, 4, 5)") {
            auto v = vector(-3, 4, 5);
            THEN("transform * v = v") {
                REQUIRE(transform * v == v);
            }
        }
    }
}


SCENARIO("A scaling matrix applied to a point.", "[transformations]")
{
    GIVEN("transform <- scaling(2, 3, 4)") {
        auto transform = scaling(2, 3, 4);
        AND_GIVEN("p <- point(-4, 6, 8)") {
            auto p = point(-4, 6, 8);
            THEN("transform * p = point(-8, 18, 32)") {
                REQUIRE(transform * p == point(-8, 18, 32));
            }
        }
    }
}


SCENARIO("A scaling matrix applied to a vector.", "[transformations]")
{
    GIVEN("transform <- scaling(2, 3, 4)") {
        auto transform = scaling(2, 3, 4);
        AND_GIVEN("v <- vector(-4, 6, 8)") {
            auto v = vector(-4, 6, 8);
            THEN("transform * v = vector(-8, 18, 32)") {
                REQUIRE(transform * v == vector(-8, 18, 32));
            }
        }
    }
}


SCENARIO("Multiplying by the inverse of a scaling matrix.", "[transformations]")
{
    GIVEN("transform <- scaling(2, 3, 4)") {
        auto transform = scaling(2, 3, 4);
        AND_GIVEN("inv <- inverse(transform)") {
            auto inv = inverse(transform);
            AND_GIVEN("v <- vector(-4, 6, 8)") {
                auto v = vector(-4, 6, 8);
                THEN("inv * v = vector(-2, 2, 2)") {
                    REQUIRE(inv * v == vector(-2, 2, 2));
                }
            }
        }
    }
}


SCENARIO("Reflection is scaling by a negative value.", "[transformations]")
{
    GIVEN("transform <- scaling(-1, 1, 1)") {
        auto transform = scaling(-1, 1, 1);
        AND_GIVEN("p <- point(2, 3, 4)") {
            auto p = point(2, 3, 4);
            THEN("transform * p = point(-2, 3, 4)") {
                REQUIRE(transform * p == point(-2, 3, 4));
            }
        }
    }
}


SCENARIO("Rotating a point around the x axis.", "[transformations]")
{
    GIVEN("p <- point(0, 1, 0)") {
        auto p = point(0, 1, 0);
        AND_GIVEN("half_quarter = rotation_x(π / 4)") {
            auto half_quarter = rotation_x(M_PI_4);
            AND_GIVEN("full_quarter = rotation_x(π / 2)") {
                auto full_quarter = rotation_x(M_PI_2);
                THEN("half_quarter * p = point(0, √2/2, √2/2)") {
                    REQUIRE(half_quarter * p == point(0, M_SQRT2 / 2, M_SQRT2 / 2));
                    AND_THEN("full_quarter * p = point(0, 0, 1)") {
                        REQUIRE(full_quarter * p == point(0, 0, 1));
                    }
                }
            }
        }
    }
}


SCENARIO("The inverse of an x-rotation rotates in the opposite direction.", "[transformations]")
{
    GIVEN("p <- point(0, 1, 0)") {
        auto p = point(0, 1, 0);
        AND_GIVEN("half_quarter = rotation_x(π / 4)") {
            auto half_quarter = rotation_x(M_PI_4);
            AND_GIVEN("inv <- inverse(half_quarter)") {
                auto inv = inverse(half_quarter);
                THEN("inv * p = point(0, √2/2, -√2/2)") {
                    REQUIRE(inv * p == point(0, M_SQRT2 / 2, -M_SQRT2 / 2));
                }
            }
        }
    }
}


SCENARIO("Rotating a point around the y axis.", "[transformations]")
{
    GIVEN("p <- point(0, 0, 1)") {
        auto p = point(0, 0, 1);
        AND_GIVEN("half_quarter = rotation_y(π/4)") {
            auto half_quarter = rotation_y(M_PI_4);
            AND_GIVEN("full_quarter = rotation_y(π/2)") {
                auto full_quarter = rotation_y(M_PI_2);
                THEN("half_quarter * p = point(√2/2, 0, √2/2)") {
                    REQUIRE(half_quarter * p == point(M_SQRT2 / 2, 0, M_SQRT2 / 2));
                    AND_THEN("full_quarter * p = point(1, 0, 0)") {
                        REQUIRE(full_quarter * p == point(1, 0, 0));
                    }
                }
            }
        }
    }
}


SCENARIO("Rotation a point around the z axis.", "[transformations]")
{
    GIVEN("p <- point(0, 1, 0)") {
        auto p = point(0, 1, 0);
        AND_GIVEN("half_quarter = rotation_z(π/4)") {
            auto half_quarter = rotation_z(M_PI_4);
            AND_GIVEN("full_quarter = rotation_z(π/2)") {
                auto full_quarter = rotation_z(M_PI_2);
                THEN("half_quarter * p = point(-√2/2, √2/2, 0)") {
                    REQUIRE(half_quarter * p == point(-M_SQRT2 / 2, M_SQRT2 / 2, 0));
                    AND_THEN("full_quarter * p = point(-1, 0, 0)") {
                        REQUIRE(full_quarter * p == point(-1, 0, 0));
                    }
                }
            }
        }
    }
}


SCENARIO("A shearing transformation moves x in proportion to y.", "[transformations]")
{
    GIVEN("transform <- shearing(1, 0, 0, 0, 0, 0)") {
        auto transform = shearing(1, 0, 0, 0, 0, 0);
        AND_GIVEN("p <- point(2, 3, 4)") {
            auto p = point(2, 3, 4);
            THEN("transform * p = point(5, 3, 4)") {
                REQUIRE(transform * p == point(5, 3, 4));
            }
        }
    }
}


SCENARIO("A shearing transformation moves x in proportion to z.", "[transformations]")
{
    GIVEN("transform <- shearing(0, 1, 0, 0, 0, 0)") {
        auto transform = shearing(0, 1, 0, 0, 0, 0);
        AND_GIVEN("p <- point(2, 3, 4)") {
            auto p = point(2, 3, 4);
            THEN("transform * p = point(6, 3, 4)") {
                REQUIRE(transform * p == point(6, 3, 4));
            }
        }
    }
}


SCENARIO("A shearing transformation moves y in proportion to x.", "[transformations]")
{
    GIVEN("transform <- shearing(0, 0, 1, 0, 0, 0)") {
        auto transform = shearing(0, 0, 1, 0, 0, 0);
        AND_GIVEN("p <- point(2, 3, 4)") {
            auto p = point(2, 3, 4);
            THEN("transform * p = point(2, 5, 4)") {
                REQUIRE(transform * p == point(2, 5, 4));
            }
        }
    }
}


SCENARIO("A shearing transformation moves y in proportion to z.", "[transformations]")
{
    GIVEN("transform <- shearing(0, 0, 0, 1, 0, 0)") {
        auto transform = shearing(0, 0, 0, 1, 0, 0);
        AND_GIVEN("p <- point(2, 3, 4)") {
            auto p = point(2, 3, 4);
            THEN("transform * p = point(2, 7, 4)") {
                REQUIRE(transform * p == point(2, 7, 4));
            }
        }
    }
}


SCENARIO("A shearing transformation moves z in proportion to x.", "[transformations]")
{
    GIVEN("transform <- shearing(0, 0, 0, 0, 1, 0)") {
        auto transform = shearing(0, 0, 0, 0, 1, 0);
        AND_GIVEN("p <- point(2, 3, 4)") {
            auto p = point(2, 3, 4);
            THEN("transform * p = point(2, 3, 6)") {
                REQUIRE(transform * p == point(2, 3, 6));
            }
        }
    }
}


SCENARIO("A shearing transformation moves z in proportion to y.", "[transformations]")
{
    GIVEN("transform <- shearing(0, 0, 0, 0, 0, 1)") {
        auto transform = shearing(0, 0, 0, 0, 0, 1);
        AND_GIVEN("p <- point(2, 3, 4)") {
            auto p = point(2, 3, 4);
            THEN("transform * p = point(2, 3, 7)") {
                REQUIRE(transform * p == point(2, 3, 7));
            }
        }
    }
}


SCENARIO("Individual transformations are applied in sequence.", "[transformations]")
{
    GIVEN("p <- point(1, 0, 1)") {
        auto p = point(1, 0, 1);
        AND_GIVEN("A <- rotation_x(π/2)") {
            auto a = rotation_x(M_PI_2);
            AND_GIVEN("B <- scaling(5, 5, 5)") {
                auto b = scaling(5, 5, 5);
                AND_GIVEN("C <- tranlstion(10, 5, 7)") {
                    auto c = translation(10, 5, 7);

                    // apply rotation first
                    WHEN("p2 <- A * p") {
                        auto p2 = a * p;
                        THEN("p2 = point(1, -1, 0)") {
                            REQUIRE(p2 == point(1, -1, 0));

                            // then apply scaling
                            AND_WHEN("p3 <- B * p2") {
                                auto p3 = b * p2;
                                THEN("p3 = point(5, -5, 0)") {
                                    REQUIRE(p3 == point(5, -5, 0));
                                }

                                // then apply translation
                                AND_WHEN("p4 <- C * p3") {
                                    auto p4 = c * p3;
                                    THEN("p4 = point(15, 0, 7)") {
                                        REQUIRE(p4 == point(15, 0, 7));
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


SCENARIO("Chained transformations must be applied in reverse order.", "[transformations]")
{
    GIVEN("p <- point(1, 0, 1)") {
        auto p = point(1, 0, 1);
        AND_GIVEN("A <- rotation(π/2)") {
            auto a = rotation_x(M_PI_2);
            AND_GIVEN("B <- scaling(5, 5, 5)") {
                auto b = scaling(5, 5, 5);
                AND_GIVEN("C <- translation(10, 5, 7)") {
                    auto c = translation(10, 5, 7);
                    WHEN("T <- C * B * A") {
                        auto t = c * b * a;
                        THEN("T * p = point(15, 0, 7)") {
                            REQUIRE(t * p == point(15, 0, 7));
                        }
                    }
                }
            }
        }
    }
}
