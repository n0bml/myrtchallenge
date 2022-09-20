/**
 * @file examples/analog.cpp
 * @brief Example program showing how to use the transformations.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>

#include <myrtchallenge/canvas.hpp>
#include <myrtchallenge/transformations.hpp>


namespace fs = std::filesystem;


int main(int argc, char* argv[], char* envp[])
{
    auto white = color(1, 1, 1);

    auto c = canvas(512, 512);
    auto half_width = c.width / 2;
    auto half_height = c.height / 2;
    auto radius = 3.0f / 8.0f * c.width;

    constexpr double_t slice = M_PI / 6.0;
    auto noon = point(0, -1, 0);

    // plot center of face
    write_pixel(c, half_width, half_height, white);

    // plot the point for each hour marker
    auto r = identity_matrix();
    for (double_t hours = 0; hours < 12; hours += 1) {
        r = rotation_z(hours * slice);
        auto marker = r * noon;
        write_pixel(c, marker.x * radius + half_width, half_height + marker.y * radius, white);
    }

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(c, png_file.string());

    return EXIT_SUCCESS;
}
