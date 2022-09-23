#pragma once

/**
 * @file include/myrtchallenge/colors.hpp
 * @brief Declaration of color and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>


struct Color
{
    double_t red{0};
    double_t green{0};
    double_t blue{0};

    bool operator==(const Color& rhs) const;
    bool operator!=(const Color& rhs) const;
};


Color color(double_t red, double_t green, double_t blue);

Color operator+(const Color& lhs, const Color& rhs);
Color operator-(const Color& lhs, const Color& rhs);
Color operator*(const Color& lhs, const Color& rhs);
Color operator*(const Color& lhs, double_t rhs);

Color normalize(Color c);
