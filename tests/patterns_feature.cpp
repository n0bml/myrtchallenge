/**
 * @file tests/patterns_feature.cpp
 * @brief Tests for patterns and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/patterns.hpp>
#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>

#include "catch_helpers.hpp"
#include "test_pattern.hpp"


// Background:
static auto black = color(0, 0, 0);
static auto white = color(1, 1, 1);


SCENARIO("The default test pattern.", "[patterns]") {
    GIVEN("pattern <- test_pattern()") {
        auto pattern = test_pattern();
        THEN("pattern.transform = identity_matrix") {
            REQUIRE(pattern->transform == identity_matrix());
        }
    }
}


SCENARIO("Assigning a transformation.", "[patterns]") {
    GIVEN("pattern <- test_pattern()") {
        auto pattern = test_pattern();
        WHEN("set_pattern_transform(pattern, translation(1, 2, 3))") {
            set_pattern_transform(pattern, translation(1, 2, 3));
            THEN("pattern->transform = translation(1, 2, 3)") {
                REQUIRE(pattern->transform == translation(1, 2, 3));
            }
        }
    }
}


SCENARIO("A pattern with an object transformation.", "[patterns]") {
    GIVEN("shape <- sphere()") {
        auto shape = sphere();
        AND_GIVEN("set_transform(shape, scaling(2, 2, 2))") {
            set_transform(shape, scaling(2, 2, 2));
            AND_GIVEN("pattern <- test_pattern()") {
                auto pattern = test_pattern();
                WHEN("c <- pattern_at_shape(pattern, shape, point(2, 3, 4)") {
                    auto c = pattern_at_shape(pattern, shape, point(2, 3, 4));
                    THEN("c = color(1, 1.5, 2)") {
                        REQUIRE(c == color(1, 1.5, 2));
                    }
                }
            }
        }
    }
}


SCENARIO("A pattern with a pattern transformation.", "[patterns]") {
    GIVEN("shape <- sphere()") {
        auto shape = sphere();
        AND_GIVEN("pattern <- test_pattern()") {
            auto pattern = test_pattern();
            AND_GIVEN("set_pattern_transform(pattern, scaling(2, 2, 2))") {
                set_pattern_transform(pattern, scaling(2, 2, 2));
                WHEN("c <- pattern_at_shape(pattern, shape, point(2, 3, 4))") {
                    auto c = pattern_at_shape(pattern, shape, point(2, 3, 4));
                    THEN("c = color(1, 1.5, 2)") {
                        REQUIRE(c == color(1, 1.5, 2));
                    }
                }
            }
        }
    }
}


SCENARIO("A pattern with both an object and a pattern transformation.", "[patterns]") {
    GIVEN("shape <- sphere()") {
        auto shape = sphere();
        AND_GIVEN("set_transform(shape, scaling(2, 2, 2))") {
            set_transform(shape, scaling(2, 2, 2));
            AND_GIVEN("pattern <- test_pattern()") {
                auto pattern = test_pattern();
                AND_GIVEN("set_pattern_transform(pattern, translation(0.5, 1, 1.5))") {
                    set_pattern_transform(pattern, translation(0.5, 1, 1.5));
                    WHEN("c <- pattern_at_shape(pattern, shape, point(2.5, 3, 3.5))") {
                        auto c = pattern_at_shape(pattern, shape, point(2.5, 3, 3.5));
                        THEN("c = color(0.75, 0.5, 0.25)") {
                            REQUIRE(c == color(0.75, 0.5, 0.25));
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Creating a stripe pattern.", "[patterns]") {
    GIVEN("pattern <- stripe_pattern(white, black)") {
        auto pattern = stripe_pattern(white, black);
        THEN("pattern.a = white") {
            REQUIRE(pattern->a == white);
            AND_THEN("pattern.b = black") {
                REQUIRE(pattern->b == black);
            }
        }
    }
}


SCENARIO("A stripe pattern is constant in y.", "[patterns]") {
    GIVEN("pattern <- stripe_pattern(white, black)") {
        auto pattern = stripe_pattern(white, black);
        THEN("stripe_at(pattern, point(0, 0, 0)) = white") {
            REQUIRE(pattern->pattern_at(point(0, 0, 0)) == white);
            AND_THEN("stripe_at(pattern, point(0, 1, 0)) = white") {
                REQUIRE(pattern->pattern_at(point(0, 1, 0)) == white);
                AND_THEN("stripe_at(pattern, point(0, 2, 0)) = white") {
                    REQUIRE(pattern->pattern_at(point(0, 2, 0)) == white);
                }
            }
        }
    }
}


SCENARIO("A stripe pattern is constant in z.", "[patterns]") {
    GIVEN("pattern <- stripe_pattern(white, black)") {
        auto pattern = stripe_pattern(white, black);
        THEN("stripe_at(pattern, point(0, 0, 0)) = white") {
            REQUIRE(pattern->pattern_at(point(0, 0, 0)) == white);
            AND_THEN("stripe_at(pattern, point(0, 0, 1)) = white") {
                REQUIRE(pattern->pattern_at(point(0, 0, 1)) == white);
                AND_THEN("stripe_at(pattern, point(0, 0, 2)) = white") {
                    REQUIRE(pattern->pattern_at(point(0, 0, 2)) == white);
                }
            }
        }
    }
}


SCENARIO("A stripe pattern alternates in x.", "[patterns]") {
    GIVEN("pattern <- stripe_pattern(white, black)") {
        auto pattern = stripe_pattern(white, black);
        THEN("stripe_at(pattern, point(0, 0, 0)) = white") {
            REQUIRE(pattern->pattern_at(point(0, 0, 0)) == white);
            AND_THEN("stripe_at(pattern, point(0.9, 0, 0) = white") {
                REQUIRE(pattern->pattern_at(point(0.9, 0, 0)) == white);
                AND_THEN("stripe_at(pattern, point(1, 0, 0)) = black") {
                    REQUIRE(pattern->pattern_at(point(1, 0, 0)) == black);
                    AND_THEN("stripe_at(pattern, point(-0.1, 0, 0)) = black") {
                        REQUIRE(pattern->pattern_at(point(-0.1, 0, 0)) == black);
                        AND_THEN("stripe_at(pattern, point(-1, 0, 0)) = black") {
                            REQUIRE(pattern->pattern_at(point(-1, 0, 0)) == black);
                            AND_THEN("stripe_at(pattern, point(-1.1, 0, 0)) = white") {
                                REQUIRE(pattern->pattern_at(point(-1.1, 0, 0)) == white);
                            }
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Stripes with an object transformation.", "[patterns]") {
    GIVEN("object <- sphere()") {
        auto object = sphere();
        AND_GIVEN("set_transform(object, scaling(2, 2, 2))") {
            set_transform(object, scaling(2, 2, 2));
            AND_GIVEN("pattern <- stripe_pattern(white, black)") {
                auto pattern = stripe_pattern(white, black);
                WHEN("c <- pattern_at_shape(pattern, object, point(1.5, 0, 0))") {
                    auto c = pattern_at_shape(pattern, object, point(1.5, 0, 0));
                    THEN("c = white") {
                        REQUIRE(c == white);
                    }
                }
            }
        }
    }
}


SCENARIO("Stripes with a pattern transformation.", "[patterns]") {
    GIVEN("object <- sphere()") {
        auto object = sphere();
        AND_GIVEN("pattern <- stripe_pattern(white, black)") {
            auto pattern = stripe_pattern(white, black);
            AND_GIVEN("set_pattern_transform(pattern, scaling(2, 2, 2))") {
                set_pattern_transform(pattern, scaling(2, 2, 2));
                WHEN("c <- pattern_at_shape(pattern, object, point(1.5, 0, 0))") {
                    auto c = pattern_at_shape(pattern, object, point(1.5, 0, 0));
                    THEN("c = white") {
                        REQUIRE(c == white);
                    }
                }
            }
        }
    }
}


SCENARIO("Stripes with both an object and a pattern transformation.", "[patterns]") {
    GIVEN("object <- sphere()") {
        auto object = sphere();
        AND_GIVEN("set_transform(object, scaling(2, 2, 2))") {
            set_transform(object, scaling(2, 2, 2));
            AND_GIVEN("pattern <- stripe_pattern(white, black)") {
                auto pattern = stripe_pattern(white, black);
                AND_GIVEN("set_pattern_transform(pattern, translation(0.5, 0, 0))") {
                    set_pattern_transform(pattern, translation(0.5, 0, 0));
                    WHEN("c <- pattern_at_shape(pattern, object, point(2.5, 0, 0))") {
                        auto c = pattern_at_shape(pattern, object, point(2.5, 0, 0));
                        THEN("c = white") {
                            REQUIRE(c == white);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("A gradient linearly interpolates between colors.", "[patterns]") {
    GIVEN("pattern <- graident_pattern(white, black)") {
        auto pattern = gradient_pattern(white, black);
        THEN("pattern_at(pattern, point(0, 0, 0)) = white") {
            REQUIRE(pattern->pattern_at(point(0, 0, 0)) == white);
            AND_THEN("pattern_at(pattern, point(0.25, 0, 0) = color(0.75, 0.75, 0.75)") {
                REQUIRE(pattern->pattern_at(point(0.25, 0, 0)) == color(0.75, 0.75, 0.75));
                AND_THEN("pattern_at(pattern, point(0.5, 0, 0)) = color(0.5, 0.5, 0.5)") {
                    REQUIRE(pattern->pattern_at(point(0.5, 0, 0)) == color(0.5, 0.5, 0.5));
                    AND_THEN("pattern_at(pattern, point(0.75, 0, 0)) = color(0.5, 0.5, 0.5)") {
                        REQUIRE(pattern->pattern_at(point(0.75, 0, 0)) == color(0.25, 0.25, 0.25));
                    }
                }
            }
        }
    }
}


SCENARIO("A ring should extend in both x and z.", "[patterns]") {
    GIVEN("pattern <- ring_pattern(white, black)") {
        auto pattern = ring_pattern(white, black);
        THEN("pattern_at(pattern, point(0, 0, 0)) = white") {
            REQUIRE(pattern->pattern_at(point(0, 0, 0)) == white);
            AND_THEN("pattern_at(pattern, point(1, 0, 0)) = black") {
                REQUIRE(pattern->pattern_at(point(1, 0, 0)) == black);
                AND_THEN("pattern_at(pattern, point(0, 0, 1)) = black") {
                    REQUIRE(pattern->pattern_at(point(0, 0, 1)) == black);
                    AND_THEN("pattern_at(pattern, point(0.708, 0, 0.708)) = black") {
                        REQUIRE(pattern->pattern_at(point(0.708, 0, 0.708)) == black);
                    }
                }
            }
        }
    }
}


SCENARIO("Checkers should repeat in x.", "[patterns]") {
    GIVEN("pattern <- checkers_pattern(white, black)") {
        auto pattern = checkers_pattern(white, black);
        THEN("pattern_at(pattern, point(0, 0, 0)) = white") {
            REQUIRE(pattern->pattern_at(point(0, 0, 0)) == white);
            AND_THEN("pattern_at(pattern, point(0.99, 0, 0)) = white") {
                REQUIRE(pattern->pattern_at(point(0.99, 0, 0)) == white);
                AND_THEN("pattern_at(pattern, point(1.01, 0, 0)) = black") {
                    REQUIRE(pattern->pattern_at(point(1.01, 0, 0)) == black);
                }
            }
        }
    }
}


SCENARIO("Checkers should repeat in y.", "[patterns]") {
    GIVEN("pattern <- checkers_pattern(white, black)") {
        auto pattern = checkers_pattern(white, black);
        THEN("pattern_at(pattern, point(0, 0, 0)) = white") {
            REQUIRE(pattern->pattern_at(point(0, 0, 0)) == white);
            AND_THEN("pattern_at(pattern, point(0, 0.99, 0)) = white") {
                REQUIRE(pattern->pattern_at(point(0, 0.99, 0)) == white);
                AND_THEN("pattern_at(pattern, point(0, 1.01, 0)) = black") {
                    REQUIRE(pattern->pattern_at(point(0, 1.01, 0)) == black);
                }
            }
        }
    }
}


SCENARIO("Checkers should repeat in z.", "[patterns]") {
    GIVEN("pattern <- checkers_pattern(white, black)") {
        auto pattern = checkers_pattern(white, black);
        THEN("pattern_at(pattern, point(0, 0, 0)) = white") {
            REQUIRE(pattern->pattern_at(point(0, 0, 0)) == white);
            AND_THEN("pattern_at(pattern, point(0, 0, 0.99)) = white") {
                REQUIRE(pattern->pattern_at(point(0, 0, 0.99)) == white);
                AND_THEN("pattern_at(pattern, point(0, 0, 1.01)) = black") {
                    REQUIRE(pattern->pattern_at(point(0, 0, 1.01)) == black);
                }
            }
        }
    }
}
