/**
 * @file src/colors.cpp
 * @brief Definition of color and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include "myrtchallenge/colors.hpp"
#include "primitives.hpp"


bool Color::operator==(const Color& rhs) const
{
    return equal(red, rhs.red) && equal(green, rhs.green) && equal(blue, rhs.blue);
}


bool Color::operator!=(const Color& rhs) const
{
    return !(*this == rhs);
}


Color color(double_t red, double_t green, double_t blue)
{
    return Color{red, green, blue};
}


Color operator+(const Color& lhs, const Color& rhs)
{
    return color(lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue);
}


Color operator-(const Color& lhs, const Color& rhs)
{
    return color(lhs.red - rhs.red, lhs.green - rhs.green, lhs.blue - rhs.blue);
}


Color operator*(const Color& lhs, const Color& rhs)
{
    return color(lhs.red * rhs.red, lhs.green * rhs.green, lhs.blue * rhs.blue);
}


Color operator*(const Color& lhs, double_t rhs)
{
    return color(lhs.red * rhs, lhs.green * rhs, lhs.blue * rhs);
}

