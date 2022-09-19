/**
 * @file src/canvas.cpp
 * @brief Definition of canvas and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <csetjmp>
#include <cstdio>
#include <memory>

#include <png.h>

#include "myrtchallenge/canvas.hpp"


Canvas canvas(size_t width, size_t height)
{
    std::vector<Color> pixels(width * height);
    return Canvas{width, height, pixels};
}


void canvas_to_png(const Canvas& canvas, std::string const& file_name)
{
    // first allocate the buffers to hold the rows of png data
    png_byte** row_pointers = new png_byte*[canvas.height];
    for (size_t row = 0; row < canvas.height; ++row)
        row_pointers[row] = new png_byte[3 * canvas.width];

    // fill the png data buffer with the canvas pixel data
    for (size_t row = 0; row < canvas.height; ++row) {
        size_t col, off;
        for (col = 0, off = 0; col < canvas.width; ++col, off += 3) {
            auto pixel = pixel_at(canvas, col, row);
            row_pointers[row][off + 0] = static_cast<png_byte>(std::ceil(pixel.red   * 255.0));
            row_pointers[row][off + 1] = static_cast<png_byte>(std::ceil(pixel.green * 255.0));
            row_pointers[row][off + 2] = static_cast<png_byte>(std::ceil(pixel.blue  * 255.0));
        }
    }

    // write the png data to the specified file
    FILE* fp = std::fopen(file_name.c_str(), "wb");
    auto png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    auto info = png_create_info_struct(png);
    png_set_rows(png, info, row_pointers);

    // @TODO(bml) add better error handling and recovery
    if (setjmp(png_jmpbuf(png))) {
        printf("DANGER WILL ROBINSON!\n");
        png_destroy_write_struct(&png, &info);
        std::fclose(fp);
        return;
    }

    png_set_IHDR(png, info, canvas.width, canvas.height, 8,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_init_io(png, fp);
    png_write_png(png, info, 0, nullptr);
    png_destroy_write_struct(&png, &info);
    std::fclose(fp);

    // release the png row buffers
    for (size_t row = 0; row < canvas.height; ++row)
        delete [] row_pointers[row];
    delete [] row_pointers;
}


Color pixel_at(const Canvas& canvas, size_t x, size_t y)
{
    return canvas.pixels.at(y * canvas.width + x);
}


void write_pixel(Canvas& canvas, size_t x, size_t y, const Color& clr)
{
    canvas.pixels[y * canvas.width + x] = clr;
}
