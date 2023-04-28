// ======================================================
// table.cpp
//
// This file describes the scene illustrated at the start
// of chapter 12, "Cubes", in "The Ray Tracer Challenge"
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

    auto cam = camera(640, 480, 0.785);
    cam->transform = view_transform(point(8, 6, -8), point(0, 3, 0), vector(0, 1, 0));

    // ======================================================
    // light sources
    // ======================================================

    w->light = point_light(point(0, 6.9, -5), color(1, 1, 0.9));

    // ======================================================
    // describe the elements of the scene
    // ======================================================

    // floor/ceiling
    auto floor = cube();
    floor->transform = scaling(20, 7, 20) * translation(0, 1, 0);
    floor->material->pattern = checkers_pattern(color(0, 0, 0), color(0.25, 0.25, 0.25));
    floor->material->pattern->transform = scaling(0.07, 0.07, 0.07);
    floor->material->ambient = 0.25;
    floor->material->diffuse = 0.7;
    floor->material->specular = 0.9;
    floor->material->shininess = 300;
    floor->material->reflective = 0.1;
    w->objects.push_back(floor);

    // walls
    auto walls = cube();
    walls->transform = scaling(10, 10, 10);
    walls->material->pattern = checkers_pattern(color(0.4863, 0.3765, 0.2941), color(0.3725, 0.2902, 0.2275));
    walls->material->pattern->transform = scaling(0.05, 20, 0.05);
    walls->material->ambient = 0.1;
    walls->material->diffuse = 0.7;
    walls->material->specular = 0.9;
    walls->material->shininess = 300;
    walls->material->reflective = 0.1;
    w->objects.push_back(walls);

    // table top
    auto table_top = cube();
    table_top->transform = translation(0, 3.1, 0) * scaling(3, 0.1, 2);
    table_top->material->pattern = stripe_pattern(color(0.5529, 0.4235, 0.3255), color(0.6588, 0.5098, 0.4000));
    table_top->material->pattern->transform = scaling(0.05, 0.05, 0.05) * rotation_y(0.1);
    table_top->material->ambient = 0.1;
    table_top->material->diffuse = 0.7;
    table_top->material->specular = 0.9;
    table_top->material->shininess = 300;
    table_top->material->reflective = 0.2;
    w->objects.push_back(table_top);

    // leg #1
    auto leg1 = cube();
    leg1->transform = translation(2.7, 1.5, -1.7) * scaling(0.1, 1.5, 0.1);
    leg1->material->color = color(0.5529, 0.4235, 0.3255);
    leg1->material->ambient = 0.2;
    leg1->material->diffuse = 0.7;
    w->objects.push_back(leg1);

    // leg #2
    auto leg2 = cube();
    leg2->transform = translation(2.7, 1.5, 1.7) * scaling(0.1, 1.5, 0.1);
    leg2->material->color = color(0.5529, 0.4235, 0.3255);
    leg2->material->ambient = 0.2;
    leg2->material->diffuse = 0.7;
    w->objects.push_back(leg2);

    // leg #3
    auto leg3 = cube();
    leg3->transform = translation(-2.7, 1.5, -1.7) * scaling(0.1, 1.5, 0.1);
    leg3->material->color = color(0.5529, 0.4235, 0.3255);
    leg3->material->ambient = 0.2;
    leg3->material->diffuse = 0.7;
    w->objects.push_back(leg3);

    // leg #4
    auto leg4 = cube();
    leg4->transform = translation(-2.7, 1.5, 1.7) * scaling(0.1, 1.5, 0.1);
    leg4->material->color = color(0.5529, 0.4235, 0.3255);
    leg4->material->ambient = 0.2;
    leg4->material->diffuse = 0.7;
    w->objects.push_back(leg4);

    // glass cube
    auto glass_cube = cube();
    glass_cube->transform = translation(0, 3.45001, 0) * rotation_y(0.2) * scaling(0.25, 0.25, 0.25);
    //glass_cube.shadow = false;  TODO - add flag to ignore shadows?
    glass_cube->material->color = color(1, 1, 0.8);
    glass_cube->material->ambient = 0;
    glass_cube->material->diffuse = 0.3;
    glass_cube->material->specular = 0.9;
    glass_cube->material->shininess = 300;
    glass_cube->material->reflective = 0.7;
    glass_cube->material->transparency = 0.7;
    glass_cube->material->refractive_index = 1.5;
    w->objects.push_back(glass_cube);

    // little cube #1
    auto little_cube1 = cube();
    little_cube1->transform = translation(1, 3.35, -0.9) * rotation_y(-0.4) * scaling(0.15, 0.15, 0.15);
    little_cube1->material->color = color(1, 0.5, 0.5);
    little_cube1->material->reflective = 0.6;
    little_cube1->material->diffuse = 0.4;
    w->objects.push_back(little_cube1);

    // little cube #2
    auto little_cube2 = cube();
    little_cube2->transform = translation(-1.5, 3.27, 0.3) * rotation_y(0.4) * scaling(0.15, 0.07, 0.15);
    little_cube2->material->color = color(1, 1, 0.5);
    w->objects.push_back(little_cube2);

    // little cube #3
    auto little_cube3 = cube();
    little_cube3->transform = translation(0, 3.25, 1) * rotation_y(0.4) * scaling(0.2, 0.05, 0.05);
    little_cube3->material->color = color(0.5, 1, 0.5);
    w->objects.push_back(little_cube3);

    // little cube #4
    auto little_cube4 = cube();
    little_cube4->transform = translation(-0.6, 3.4, -1) * rotation_y(0.8) * scaling(0.05, 0.2, 0.05);
    little_cube4->material->color = color(0.5, 0.5, 1);
    w->objects.push_back(little_cube4);

    // little cube #5
    auto little_cube5 = cube();
    little_cube5->transform = translation(2, 3.4, 1) * rotation_y(0.8) * scaling(0.05, 0.2, 0.05);
    little_cube5->material->color = color(0.5, 1, 1);
    w->objects.push_back(little_cube5);

    // frame #1
    auto frame1 = cube();
    frame1->transform = translation(-10, 4, 1) * scaling(0.05, 1, 1);
    frame1->material->color = color(0.7098, 0.2471, 0.2196);
    frame1->material->diffuse = 0.6;
    w->objects.push_back(frame1);

    // frame #2
    auto frame2 = cube();
    frame2->transform = translation(-10, 3.4, 2.7) * scaling(0.05, 0.4, 0.4);
    frame2->material->color = color(0.2667, 0.2706, 0.6902);
    frame2->material->diffuse = 0.6;
    w->objects.push_back(frame2);

    // frame #3
    auto frame3 = cube();
    frame3->transform = translation(-10, 4.5, 2.7) * scaling(0.05, 0.4, 0.4);
    frame3->material->color = color(0.3098, 0.5961, 0.3098);
    frame3->material->diffuse = 0.6;
    w->objects.push_back(frame3);

    // mirror frame
    auto mirror_frame = cube();
    mirror_frame->transform = translation(-2, 3.5, 9.95) * scaling(5, 1.5, 0.05);
    mirror_frame->material->color = color(0.3882, 0.2627, 0.1882);
    mirror_frame->material->diffuse = 0.7;
    w->objects.push_back(mirror_frame);

    // mirror
    auto mirror = cube();
    mirror->transform = translation(-2, 3.5, 9.95) * scaling(4.8, 1.4, 0.06);
    mirror->material->color = color(0, 0, 0);
    mirror->material->diffuse = 0;
    mirror->material->ambient = 0;
    mirror->material->specular = 1;
    mirror->material->shininess = 300;
    mirror->material->reflective = 1;
    w->objects.push_back(mirror);

    auto image = render(cam, w);

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(image, png_file.string());

    return EXIT_SUCCESS;
}
