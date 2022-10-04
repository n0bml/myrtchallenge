/**
 * @file examples/planes.cpp
 * @brief Example program showing how to use plane shapes.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cstdio>
#include <cstdlib>
#include <filesystem>

#include <myrtchallenge/camera.hpp>
#include <myrtchallenge/shapes.hpp>
#include <myrtchallenge/transformations.hpp>
#include <myrtchallenge/world.hpp>


namespace fs = std::filesystem;


int main(int argc, char* argv[], char* envp[])
{
    auto w = world();
    w->light = point_light(point(-10, 10, -10), color(1, 1, 1));

    auto floor = plane();
    w->objects.push_back(floor);

    auto middle = sphere();
    middle->transform = translation(-0.5, 1, 0.5);
    middle->material->color = color(0.1, 1, 0.5);
    middle->material->diffuse = 0.7;
    middle->material->specular = 0.3;
    w->objects.push_back(middle);

    auto right = sphere();
    right->transform = translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5);
    right->material->color = color(0.5, 1, 0.1);
    right->material->diffuse = 0.7;
    right->material->specular = 0.3;
    w->objects.push_back(right);

    auto left = sphere();
    left->transform = translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33);
    left->material->color = color(1, 0.8, 0.1);
    left->material->diffuse = 0.7;
    left->material->specular = 0.3;
    w->objects.push_back(left);

    auto cam = camera(640, 480, M_PI / 3);
    cam->transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

    auto canvas = render(cam, w);

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(canvas, png_file.string());

    return EXIT_SUCCESS;
}
