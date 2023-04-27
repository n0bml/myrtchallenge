// ======================================================
// reflect-refract.cpp
//
// This file describes the scene illustrated at the start
// of chapter 11, "Reflection and Refraction", in "The
// Ray Tracer Challenge"
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
    auto cam = camera(640, 480, 1.152);
    cam->transform = view_transform(point(-2.6, 1.5, -3.9), point(-0.6, 1, -0.8), vector(0, 1, 0));

    // ======================================================
    // light sources
    // ======================================================
    w->light = point_light(point(-4.9, 4.9, 1), color(1, 1, 1));

    // ======================================================
    // define constants to avoid duplication
    // ======================================================

    auto wall_material = material();
    wall_material->pattern            = stripe_pattern(color(0.45, 0.45, 0.45), color(0.55, 0.55, 0.55));
    wall_material->pattern->transform = rotation_y(1.5708) * scaling(0.25, 0.25, 0.25);
    wall_material->ambient            = 0;
    wall_material->diffuse            = 0.4;
    wall_material->specular           = 0;
    wall_material->reflective         = 0.3;

    // ======================================================
    // describe the elements of the scene
    // ======================================================

    auto floor = plane();
    floor->transform            = rotation_y(0.31415);
    floor->material->pattern    = checkers_pattern(color(0.35, 0.35, 0.35), color(0.65, 0.65, 0.65));
    floor->material->specular   = 0;
    floor->material->reflective = 0.4;
    w->objects.push_back(floor);

    auto ceiling = plane();
    ceiling->transform          = translation(0, 5, 0);
    ceiling->material->color    = color(0.8, 0.8, 0.8);
    ceiling->material->ambient  = 0.3;
    ceiling->material->specular = 0;
    w->objects.push_back(ceiling);

    auto west_wall = plane();
    west_wall->transform = translation(-5, 0, 0) * rotation_z(1.5708) * rotation_y(1.5708);
    west_wall->material  = wall_material;
    w->objects.push_back(west_wall);

    auto east_wall = plane();
    east_wall->transform = translation(5, 0, 0) * rotation_z(1.5708) * rotation_y(1.5708);
    east_wall->material  = wall_material;
    w->objects.push_back(east_wall);

    auto north_wall = plane();
    north_wall->transform = translation(0, 0, 5) * rotation_x(1.5708);
    north_wall->material  = wall_material;
    w->objects.push_back(north_wall);

    auto south_wall = plane();
    south_wall->transform = translation(0, 0, -5) * rotation_x(1.5708);
    south_wall->material = wall_material;
    w->objects.push_back(south_wall);

    // ----------------------
    // background balls
    // ----------------------

    auto bb1 = sphere();
    bb1->transform           = translation(4.6, 0.4, 1) * scaling(0.4, 0.4, 0.4);
    bb1->material->color     = color(0.8, 0.5, 0.3);
    bb1->material->shininess = 50;
    w->objects.push_back(bb1);

    auto bb2 = sphere();
    bb2->transform           = translation(4.7, 0.3, 0.4) * scaling(0.3, 0.3, 0.3);
    bb2->material->color     = color(0.9, 0.4, 0.5);
    bb2->material->shininess = 50;
    w->objects.push_back(bb2);

    auto bb3 = sphere();
    bb3->transform           = translation(-1, 0.5, 4.5) * scaling(0.5, 0.5, 0.5);
    bb3->material->color     = color(0.4, 0.9, 0.6);
    bb3->material->shininess = 50;
    w->objects.push_back(bb3);

    auto bb4 = sphere();
    bb4->transform           = translation(-1.7, 0.3, 4.7) * scaling(0.3, 0.3, 0.3);
    bb4->material->color     = color(0.4, 0.6, 0.9);
    bb4->material->shininess = 50;
    w->objects.push_back(bb4);

    // ----------------------
    // foreground balls
    // ----------------------

    auto red_sphere = sphere();
    red_sphere->transform = translation(-0.6, 1, 0.6);
    red_sphere->material->color     = color(1, 0.3, 0.2);
    red_sphere->material->specular  = 0.4;
    red_sphere->material->shininess = 5;

    auto blue_glass_sphere = sphere();
    blue_glass_sphere->transform = translation(0.6, 0.7, -0.6) * scaling(0.7, 0.7, 0.7);
    blue_glass_sphere->material->color            = color(0, 0, 0.2);
    blue_glass_sphere->material->ambient          = 0;
    blue_glass_sphere->material->diffuse          = 0.4;
    blue_glass_sphere->material->specular         = 0.9;
    blue_glass_sphere->material->shininess        = 300;
    blue_glass_sphere->material->reflective       = 0.9;
    blue_glass_sphere->material->transparency     = 0.9;
    blue_glass_sphere->material->refractive_index = 1.5;
    w->objects.push_back(blue_glass_sphere);

    auto green_glass_sphere = sphere();
    green_glass_sphere->transform = translation(-0.7, 0.5, -0.8) * scaling(0.5, 0.5, 0.5);
    green_glass_sphere->material->color            = color(0, 0.2, 0);
    green_glass_sphere->material->ambient          = 0;
    green_glass_sphere->material->diffuse          = 0.4;
    green_glass_sphere->material->specular         = 0.9;
    green_glass_sphere->material->shininess        = 300;
    green_glass_sphere->material->reflective       = 0.9;
    green_glass_sphere->material->transparency     = 0.9;
    green_glass_sphere->material->refractive_index = 1.5;
    w->objects.push_back(green_glass_sphere);

    auto image = render(cam, w);

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(image, png_file.string());

    return EXIT_SUCCESS;
}
