/**
 * @file examples/scene.cpp
 * @brief Example program showing how to render a scene using the world and camera.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cstdio>
#include <cstdlib>
#include <filesystem>

#include <myrtchallenge/camera.hpp>
#include <myrtchallenge/intersections.hpp>
#include <myrtchallenge/spheres.hpp>
#include <myrtchallenge/transformations.hpp>


namespace fs = std::filesystem;


int main(int argc, char* argv[], char* envp[])
{
    auto floor = sphere();
    floor->transform = scaling(10, 0.01, 10);
    floor->material->color = color(1, 0.9, 0.9);
    floor->material->specular = 0;

    auto left_wall = sphere();
    left_wall->transform = translation(0, 0, 5) * rotation_y(-M_PI_4) * rotation_x(M_PI_2) * scaling(10, 0.01, 10);
    left_wall->material = floor->material;

    auto right_wall = sphere();
    right_wall->transform = translation(0, 0, 5) * rotation_y(M_PI_4) * rotation_x(M_PI_2) * scaling(10, 0.01, 10);
    right_wall->material = floor->material;

    auto middle = sphere();
    middle->transform = translation(-0.5, 1, 0.5);
    middle->material->color = color(0.1, 1, 0.5);
    middle->material->diffuse = 0.7;
    middle->material->specular = 0.3;

    auto right = sphere();
    right->transform = translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5);
    right->material->color = color(0.5, 1, 0.1);
    right->material->diffuse = 0.7;
    right->material->specular = 0.3;

    auto left = sphere();
    left->transform = translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33);
    left->material->color = color(1, 0.8, 0.1);
    left->material->diffuse = 0.7;
    left->material->specular = 0.3;

    auto w = default_world();
    w->objects = {floor, left_wall, right_wall, middle, right, left};

    auto cam = camera(640, 480, M_PI / 3);
    cam->transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

    auto image = render(cam, w);

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(image, png_file.string());

    return EXIT_SUCCESS;
}
