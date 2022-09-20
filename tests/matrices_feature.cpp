/**
 * @file tests/matrices_feature.cpp
 * @brief Tests for matrix and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <algorithm>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <myrtchallenge/matrices.hpp>


TEST_CASE("Constructing and inspecting a 4x4 matrix.", "[matrices]")
{
    auto m = matrix(4, {
         1,    2,    3,    4,
         5.5,  6.5,  7.5,  8.5,
         9,   10,   11,   12,
        13.5, 14.5, 15.5, 16.5,
    });

    REQUIRE(m(0, 0) == 1);
    REQUIRE(m(0, 3) == 4);
    REQUIRE(m(1, 0) == 5.5);
    REQUIRE(m(1, 2) == 7.5);
    REQUIRE(m(2, 2) == 11);
    REQUIRE(m(3, 0) == 13.5);
    REQUIRE(m(3, 2) == 15.5);
}


TEST_CASE("A 2x2 matrix should be representable.", "[matrices]")
{
    auto m = matrix(2, {
        -3,  5,
         1, -2,
    });

    REQUIRE(m(0, 0) == -3);
    REQUIRE(m(0, 1) == 5);
    REQUIRE(m(1, 0) == 1);
    REQUIRE(m(1, 1) == -2);
}


TEST_CASE("A 3x3 matrix should be representable.", "[matrices]")
{
    auto m = matrix(3, {
        -3,  5,  0,
         1, -2, -7,
         0,  1,  1,
    });

    REQUIRE(m(1, 1) == -2);
    REQUIRE(m(2, 2) == 1);
}


TEST_CASE("Matrix equality with identical matrices.", "[matrices]")
{
    auto a = matrix(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2,
    });

    auto b = matrix(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2,
    });

    REQUIRE(a == b);
}


TEST_CASE("Matrix equality with different matrices.", "[matrices]")
{
    auto a = matrix(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2,
    });

    auto b = matrix(4, {
        2, 3, 4, 5,
        6, 7, 8, 9,
        8, 7, 6, 5,
        4, 3, 2, 1,
    });

    REQUIRE(a != b);
}


TEST_CASE("Multiplying two matrices.", "[matrices]")
{
    auto a = matrix(4, {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2,
    });

    auto b = matrix(4, {
        -2,  1,  2,  3,
         3,  2,  1, -1,
         4,  3,  6,  5,
         1,  2,  7,  8,
    });

    REQUIRE(a * b == matrix(4, {
        20,  22,  50,  48,
        44,  54, 114, 108,
        40,  58, 110, 102,
        16,  26,  46,  42,
    }));
}


TEST_CASE("A matrix multiplied by a tuple.", "[matrices]")
{
    auto a = matrix(4, {
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1,
    });

    auto b = tuple(1, 2, 3, 1);

    REQUIRE(a * b == tuple(18, 24, 33, 1));
}


TEST_CASE("Multiplying a matrix by the identity matrix.", "[matrices]")
{
    auto a = matrix(4, {
        0,  1,  2,  4,
        1,  2,  4,  8,
        2,  4,  8, 16,
        4,  8, 16, 32,
    });

    REQUIRE(a * identity_matrix() == a);
}


TEST_CASE("Transposing a matrix.", "[matrices]")
{
    auto a = matrix(4, {
        0, 9, 3, 0,
        9, 8, 0, 8,
        1, 8, 5, 3,
        0, 0, 5, 8,
    });

    REQUIRE(transpose(a) == matrix(4, {
        0, 9, 1, 0,
        9, 8, 8, 0,
        3, 0, 5, 5,
        0, 8, 3, 8,
    }));
}


TEST_CASE("Transposing the identity matrix.", "[matrices]")
{
    auto a = transpose(identity_matrix());
    REQUIRE(a == identity_matrix());
}


TEST_CASE("Calculating the determinant of a 2x2 matrix.", "[matrices]")
{
    auto a = matrix(2, {
         1,  5,
        -3,  2,
    });

    REQUIRE(determinant(a) == 17);
}


TEST_CASE("A submatrix of a 3x3 matrix is a 2x2 matrix.", "[matrices]")
{
    auto a = matrix(3, {
         1,  5,  0,
        -3,  2,  7,
         0,  6, -3,
    });

    REQUIRE(submatrix(a, 0, 2) == matrix(2, {
        -3,  2,
         0,  6,
    }));
}


TEST_CASE("A submatrix of a 4x4 matrix is a 3x3 matrix.", "[matrices]")
{
    auto a = matrix(4, {
        -6,  1,  1,  6,
        -8,  5,  8,  6,
        -1,  0,  8,  2,
        -7,  1, -1,  1,
    });

    REQUIRE(submatrix(a, 2, 1) == matrix(3, {
        -6,  1,  6,
        -8,  8,  6,
        -7, -1,  1,
    }));
}


TEST_CASE("Calculating a minor of a 3x3 matrix.", "[matrices]")
{
    auto a = matrix(3, {
        3,  5,  0,
        2, -1, -7,
        6, -1,  5,
    });

    auto b = submatrix(a, 1, 0);

    REQUIRE(determinant(b) == 25);
    REQUIRE(minor(a, 1, 0) == 25);
}


TEST_CASE("Calculating a cofactor of a 3x3 matrix.", "[matrices]")
{
    auto a = matrix(3, {
        3,  5,  0,
        2, -1, -7,
        6, -1,  5,
    });

    REQUIRE(minor(a, 0, 0) == -12);
    REQUIRE(cofactor(a, 0, 0) == -12);
    REQUIRE(minor(a, 1, 0) == 25);
    REQUIRE(cofactor(a, 1, 0) == -25);
}


TEST_CASE("Calculating the determinant of a 3x3 matrix.", "[matrices]")
{
    auto a = matrix(3, {
         1,  2,  6,
        -5,  8, -4,
         2,  6,  4,
    });

    REQUIRE(cofactor(a, 0, 0) == 56);
    REQUIRE(cofactor(a, 0, 1) == 12);
    REQUIRE(cofactor(a, 0, 2) == -46);
    REQUIRE(determinant(a) == -196);
}


TEST_CASE("Calculating the determinant of a 4x4 matrix.", "[matrices]")
{
    auto a = matrix(4, {
        -2, -8,  3,  5,
        -3,  1,  7,  3,
         1,  2, -9,  6,
        -6,  7,  7, -9,
    });

    REQUIRE(cofactor(a, 0, 0) == 690);
    REQUIRE(cofactor(a, 0, 1) == 447);
    REQUIRE(cofactor(a, 0, 2) == 210);
    REQUIRE(cofactor(a, 0, 3) == 51);
    REQUIRE(determinant(a) == -4071);
}


TEST_CASE("Testing an invertible matrix for invertibility.", "[matrices]")
{
    auto a = matrix(4, {
        6,  4,  4,  4,
        5,  5,  7,  6,
        4, -9,  3, -7,
        9,  1,  7, -6,
    });

    REQUIRE(determinant(a) == -2120);
    REQUIRE(invertible(a));
}


TEST_CASE("Testing a non-invertible matrix for invertibility.", "[matrices]")
{
    auto a = matrix(4, {
        -4,  2, -2, -3,
         9,  6,  2,  6,
         0, -5,  1, -5,
         0,  0,  0,  0,
    });

    REQUIRE(determinant(a) == 0);
    REQUIRE(!invertible(a));
}


TEST_CASE("Calculating the inverse of a matrix.", "[matrices]")
{
    auto a = matrix(4, {
        -5,  2,  6, -8,
         1, -5,  1,  8,
         7,  7, -6, -7,
         1, -3,  7,  4,
    });

    auto b = inverse(a);

    REQUIRE(determinant(a) == 532);
    REQUIRE(cofactor(a, 2, 3) == -160);
    REQUIRE(b(3, 2) == -160.0 / 532.0);
    REQUIRE(cofactor(a, 3, 2) == 105);
    REQUIRE(b(2, 3) == 105.0 / 532.0);
    REQUIRE(b == matrix(4, {
         0.21805,  0.45113,  0.24060, -0.04511,
        -0.80827, -1.45677, -0.44361,  0.52068,
        -0.07895, -0.22368, -0.05263,  0.19737,
        -0.52256, -0.81391, -0.30075,  0.30639,
    }));
}


TEST_CASE("Calculating the inverse of another matrix.", "[matrices]")
{
    auto a = matrix(4, {
         8, -5,  9,  2,
         7,  5,  6,  1,
        -6,  0,  9,  6,
        -3,  0, -9, -4,
    });

    REQUIRE(inverse(a) == matrix(4, {
        -0.15385, -0.15385, -0.28205, -0.53846,
        -0.07692,  0.12308,  0.02564,  0.03077,
         0.35897,  0.35897,  0.43590,  0.92308,
        -0.69231, -0.69231, -0.76923, -1.92308,
    }));
}


TEST_CASE("Calculating the inverse of a third matrix.", "[matrices]")
{
    auto a = matrix(4, {
         9,  3,  0,  9,
        -5, -2, -6, -3,
        -4,  9,  6,  4,
        -7,  6,  6,  2,
    });

    REQUIRE(inverse(a) == matrix(4, {
        -0.04074, -0.07778,  0.14444, -0.22222,
        -0.07778,  0.03333,  0.36667, -0.33333,
        -0.02901, -0.14630, -0.10926,  0.12963,
         0.17778,  0.06667, -0.26667,  0.33333,
    }));
}


TEST_CASE("Multiplying a product by its inverse.", "[matrices]")
{
    auto a = matrix(4, {
         3, -9,  7,  3,
         3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1,
    });

    auto b = matrix(4, {
        8, -2,  2,  2,
        3, -1,  7,  0,
        7,  0,  5,  4,
        6, -2,  0,  5,
    });

    auto c = a * b;

    REQUIRE(c * inverse(b) == a);
}
