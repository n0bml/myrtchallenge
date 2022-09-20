#pragma once

/**
 * @file include/myrtchallenge/matrices.hpp
 * @brief Declaration of matrix and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <vector>

#include "myrtchallenge/tuples.hpp"

struct Matrix
{
    size_t size;
    std::vector<double_t> values;

    bool operator==(const Matrix& rhs) const;
    double_t operator()(size_t row, size_t col) const;
    double_t& operator()(size_t row, size_t col);
};


Matrix matrix(size_t sz);
Matrix matrix(size_t sz, const std::vector<double_t>& initial);

Matrix operator*(const Matrix& lhs, const Matrix& rhs);
Tuple operator*(const Matrix& m, const Tuple& t);

double_t cofactor(const Matrix& m, size_t row, size_t col);
double_t determinant(const Matrix& m);
Matrix identity_matrix();
Matrix inverse(const Matrix& m);
bool invertible(const Matrix& m);
double_t minor(const Matrix& m, size_t row, size_t col);
Matrix submatrix(const Matrix& m, size_t rrow, size_t rcol);
Matrix transpose(const Matrix& m);
