/**
 * @file examples/shading.cpp
 * @brief Example program showing how to use light and shading to display a sphere.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cstdio>
#include <cstdlib>
#include <filesystem>

#include <myrtchallenge/canvas.hpp>
#include <myrtchallenge/intersections.hpp>
#include <myrtchallenge/rays.hpp>
#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>


namespace fs = std::filesystem;


int main(int argc, char* argv[], char* envp[])
{
    // start the ray at z = -5
    auto ray_origin = point(0, 0, -5);

    // put the wall at z = 10
    double_t wall_z{10};
    double_t wall_size{7};
    size_t canvas_pixels{512};

    auto pixel_size = wall_size / canvas_pixels;
    auto half = wall_size / 2;

    auto c = canvas(canvas_pixels, canvas_pixels);
    auto shape = sphere();
    shape->material->color = color(1, 0.2, 1);

    auto light_position = point(-10, 10, -10);
    auto light_color = color(1, 1, 1);
    auto light = point_light(light_position, light_color);

    // for each row of pixels in the canvas
    for (size_t y = 0; y < canvas_pixels; ++y) {
        // compute the world y coordinate (top = +half, bottom = -half)
        auto world_y = half - pixel_size * y;

        // for each pixel in the row
        for (size_t x = 0; x < canvas_pixels; ++x) {
            // compute the world x coordinate (left = -half, right = +half)
            auto world_x = -half + pixel_size * x;

            // describe the point on the wall that the ray will target
            auto pos = point(world_x, world_y, wall_z);

            auto r = ray(ray_origin, normalize(pos - ray_origin));
            auto xs = intersect(shape, r);

            auto h = hit(xs);
            if (h.hit()) {
                auto pt = position(r, h.t);
                auto normal = normal_at(h.object, pt);
                auto eye = -r.direction;
                auto color = lighting(h.object->material, light, pt, eye, normal, false);
                write_pixel(c, x, y, color);
            }
        }
    }

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(c, png_file.string());

    return EXIT_SUCCESS;
}
