// ======================================================
// cylinders.yml
//
// This file describes the scene illustrated at the start
// of chapter 13, "Cylinders", in "The Ray Tracer
// Challenge"
//
// by Jamis Buck <jamis@jamisbuck.org>
//
// Translated to my implementation in C++20 by
// Brendan Leber <brendan@brendanleber.com>
// ======================================================

#include <cstdio>
#include <cstdlib>
#include <filesystem>

#include "myrtchallenge/camera.hpp"
#include "myrtchallenge/shapes.hpp"
#include "myrtchallenge/transformations.hpp"
#include "myrtchallenge/world.hpp"


namespace fs = std::filesystem;


int main(int argc, char* argv[], char* envp[])
{
    auto w = world();

    // ======================================================
    // the camera
    // ======================================================

    auto cam = camera(640, 480, 0.314);
    cam->transform = view_transform(point(8, 3.5, -9), point(0, 0.3, 0), vector(0, 1, 0));

    // ======================================================
    // light sources
    // ======================================================
    w->light = point_light(point(1, 6.9, -4.9), color(1, 1, 1));

    // ======================================================
    // describe the elements of the scene
    // ======================================================

    // floor
    auto floor = plane();
    floor->material->pattern = checkers_pattern(color(0.5, 0.5, 0.5), color(0.75, 0.75, 0.75));
    floor->material->pattern->transform = rotation_y(0.3) * scaling(0.25, 0.25, 0.25);
    floor->material->ambient = 0.2;
    floor->material->diffuse = 0.9;
    floor->material->specular = 0;
    w->objects.push_back(floor);

    auto cyl = cylinder();
    cyl->minimum = 0;
    cyl->maximum = 0.75;
    cyl->closed = true;
    cyl->transform = translation(-1, 0, 1) * scaling(0.5, 1, 0.5);
    cyl->material->color = color(0, 0, 0.6);
    cyl->material->diffuse = 0.1;
    cyl->material->specular = 0.9;
    cyl->material->shininess = 300;
    cyl->material->reflective = 0.9;
    w->objects.push_back(cyl);

    // concentric cylinders

    auto cyl2 = cylinder();
    cyl2->minimum = 0;
    cyl2->maximum = 0.2;
    cyl2->closed = false;
    cyl2->transform = translation(1, 0, 0) * scaling(0.8, 1, 0.8);
    cyl2->material->color = color(1, 1, 0.3);
    cyl2->material->ambient = 0.1;
    cyl2->material->diffuse = 0.8;
    cyl2->material->specular = 0.9;
    cyl2->material->shininess = 300;
    w->objects.push_back(cyl2);

    auto cyl3 = cylinder();
    cyl3->minimum = 0;
    cyl3->maximum = 0.3;
    cyl3->closed = false;
    cyl3->transform = translation(1, 0, 0) * scaling(0.6, 1, 0.6);
    cyl3->material->color = color(1, 0.9, 0.4);
    cyl3->material->ambient = 0.1;
    cyl3->material->diffuse = 0.8;
    cyl3->material->specular = 0.9;
    cyl3->material->shininess = 300;
    w->objects.push_back(cyl3);

    auto cyl4 = cylinder();
    cyl4->minimum = 0;
    cyl4->maximum = 0.4;
    cyl4->closed = false;
    cyl4->transform = translation(1, 0, 0) * scaling(0.4, 1, 0.4);
    cyl4->material->color = color(1, 0.8, 0.5);
    cyl4->material->ambient = 0.1;
    cyl4->material->diffuse = 0.8;
    cyl4->material->specular = 0.9;
    cyl4->material->shininess = 300;
    w->objects.push_back(cyl4);

    auto cyl5 = cylinder();
    cyl5->minimum = 0;
    cyl5->maximum = 0.5;
    cyl5->closed = true;
    cyl5->transform = translation(1, 0, 0) * scaling(0.2, 1, 0.2);
    cyl5->material->color = color(1, 0.7, 0.6);
    cyl5->material->ambient = 0.1;
    cyl5->material->diffuse = 0.8;
    cyl5->material->specular = 0.9;
    cyl5->material->shininess = 300;
    w->objects.push_back(cyl5);

    // decorative cylinders
    auto cyl6 = cylinder();
    cyl6->minimum = 0;
    cyl6->maximum = 0.3;
    cyl6->closed = true;
    cyl6->transform = translation(0, 0, -0.75) * scaling(0.05, 1, 0.05);
    cyl6->material->color = color(1, 0, 0);
    cyl6->material->ambient = 0.1;
    cyl6->material->diffuse = 0.9;
    cyl6->material->specular = 0.9;
    cyl6->material->shininess = 300;
    w->objects.push_back(cyl6);

    auto cyl7 = cylinder();
    cyl7->minimum = 0;
    cyl7->maximum = 0.3;
    cyl7->closed = true;
    cyl7->transform = translation(0, 0, -2.25) * rotation_y(-0.15) * translation(0, 0, 1.5) * scaling(0.05, 1, 0.05);
    cyl7->material->color = color(1, 1, 0);
    cyl7->material->ambient = 0.1;
    cyl7->material->diffuse = 0.9;
    cyl7->material->specular = 0.9;
    cyl7->material->shininess = 300;
    w->objects.push_back(cyl7);

    auto cyl8 = cylinder();
    cyl8->minimum = 0;
    cyl8->maximum = 0.3;
    cyl8->closed = true;
    cyl8->transform = translation(0, 0, -2.25) * rotation_y(-0.3) * translation(0, 0, 1.5) * scaling(0.05, 1, 0.05);
    cyl8->material->color = color(0, 1, 0);
    cyl8->material->ambient = 0.1;
    cyl8->material->diffuse = 0.9;
    cyl8->material->specular = 0.9;
    cyl8->material->shininess = 300;
    w->objects.push_back(cyl8);

    auto cyl9 = cylinder();
    cyl9->minimum = 0;
    cyl9->maximum = 0.3;
    cyl9->closed = true;
    cyl9->transform = translation(0, 0, -2.25) * rotation_y(-0.45) * translation(0, 0, 1.5) * scaling(0.05, 1, 0.05);
    cyl9->material->color = color(0, 1, 1);
    cyl9->material->ambient = 0.1;
    cyl9->material->diffuse = 0.9;
    cyl9->material->specular = 0.9;
    cyl9->material->shininess = 300;
    w->objects.push_back(cyl9);

    // # glass cylinder
    auto glass = cylinder();
    glass->minimum = 0.0001;
    glass->maximum = 0.5;
    glass->closed = true;
    glass->transform = translation(0, 0, -1.5) * scaling(0.33, 1, 0.33);
    glass->material->color = color(0.25, 0, 0);
    glass->material->diffuse = 0.1;
    glass->material->specular = 0.9;
    glass->material->shininess = 300;
    glass->material->transparency = 0.9;
    glass->material->refractive_index = 1.5;
    w->objects.push_back(glass);

    auto image = render(cam, w);

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(image, png_file.string());

    return EXIT_SUCCESS;
}
