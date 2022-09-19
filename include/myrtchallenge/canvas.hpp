#pragma once

/**
 * @file include/myrtchallenge/canvas.hpp
 * @brief Declaration of canvas and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cstdint>
#include <string>
#include <vector>
#include "myrtchallenge/colors.hpp"


struct Canvas
{
    size_t width;
    size_t height;
    std::vector<Color> pixels;
};


Canvas canvas(size_t width, size_t height);

void canvas_to_png(const Canvas& canvas, std::string const& file_name);
Color pixel_at(const Canvas& canvas, size_t x, size_t y);
void write_pixel(Canvas& canvas, size_t x, size_t y, const Color& clr);
