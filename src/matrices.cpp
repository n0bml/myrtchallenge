/**
 * @file src/matrices.cpp
 * @brief Definition of matrix and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "myrtchallenge/matrices.hpp"

#include "primitives.hpp"


bool Matrix::operator==(const Matrix& rhs) const
{
    if (size != rhs.size)
        return false;
    for (size_t off = 0; off < size * size; ++off)
        if (!equal(values[off], rhs.values[off]))
            return false;
    return true;
}


double_t Matrix::operator()(size_t row, size_t col) const
{
    return values.at(col + row * size);
}


double_t& Matrix::operator()(size_t row, size_t col)
{
    return values.at(col + row * size);
}


Matrix matrix(size_t sz)
{
    auto m = Matrix{sz, std::vector<double_t>(sz * sz)};
    return m;
}


Matrix matrix(size_t sz, const std::vector<double_t>& initial)
{
    return Matrix{sz, initial};
}


Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    auto m = matrix(lhs.size);
    for (size_t row = 0; row < lhs.size; ++row)
        for (size_t col = 0; col < rhs.size; ++ col) {
            double_t element = 0;
            for (size_t i = 0; i < lhs.size; ++i)
                element += lhs(row, i) * rhs(i, col);
            m(row, col) = element;
        }
    return m;
}


Tuple operator*(const Matrix& m, const Tuple& t)
{
    return tuple(
        m(0, 0) * t.x + m(0, 1) * t.y + m(0, 2) * t.z + m(0, 3) * t.w,
        m(1, 0) * t.x + m(1, 1) * t.y + m(1, 2) * t.z + m(1, 3) * t.w,
        m(2, 0) * t.x + m(2, 1) * t.y + m(2, 2) * t.z + m(2, 3) * t.w,
        m(3, 0) * t.x + m(3, 1) * t.y + m(3, 2) * t.z + m(3, 3) * t.w
    );
}


double_t cofactor(const Matrix& m, size_t row, size_t col)
{
    double_t sign = ((row + col) % 2) ? -1 : 1;
    return sign * minor(m, row, col);
}


double_t determinant(const Matrix& m)
{
    double_t det = 0;
    if (m.size == 2)
        det = m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
    else
        for (size_t column = 0; column < m.size; ++column)
            det += m(0, column) * cofactor(m, 0, column);
    return det;
}


Matrix identity_matrix()
{
    return matrix(4, {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    });
}


Matrix inverse(const Matrix& m)
{
    if (!invertible(m))
        throw std::domain_error("inverse() called with a non-invertible matrix.");

    auto inv = m;
    auto d = determinant(m);
    for (size_t row = 0; row < m.size; ++row)
        for (size_t col = 0; col < m.size; ++col) {
            // note that "col, row" here, instead of "row, col", accomplishes the transpose operation
            inv(col, row) = cofactor(m, row, col) / d;
        }
    return inv;
}


bool invertible(const Matrix& m)
{
    return !equal(determinant(m), 0);
}


double_t minor(const Matrix& m, size_t row, size_t col)
{
    return determinant(submatrix(m, row, col));
}


Matrix submatrix(const Matrix& m, size_t rrow, size_t rcol)
{
    auto s = matrix(m.size - 1);
    size_t srow = 0;
    for (size_t mrow = 0; mrow < m.size; ++mrow) {
        if (mrow == rrow)
            continue;

        size_t scol = 0;
        for (size_t mcol = 0; mcol < m.size; ++mcol) {
            if (mcol == rcol)
                continue;

            s(srow, scol) = m(mrow, mcol);
            ++scol;
        }

        ++srow;
    }

    return s;
}


Matrix transpose(const Matrix& m)
{
    auto t = matrix(m.size);
    for (size_t row = 0; row < m.size; ++row)
        for (size_t col = 0; col < m.size; ++col)
            t(col, row) = m(row, col);
    return t;
}
