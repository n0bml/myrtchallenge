// ======================================================
// puppets.cpp
//
// This file describes the scene illustrated at the end
// of chapter 8, "Shadows", in "The Ray Tracer Challenge"
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
    auto cam = camera(640, 480, 0.524);
    cam->transform = view_transform(point(40, 0, -70), point(0, 0, -5), vector(0, 1, 0));

    // ======================================================
    // light sources
    // ======================================================
    w->light = point_light(point(0, 0, -100), color(1, 1, 1));

    // ======================================================
    // define some constants to avoid duplication
    // ======================================================
    auto sphere_material = material();
    sphere_material->ambient = 0.2;
    sphere_material->diffuse = 0.8;
    sphere_material->specular = 0.3;
    sphere_material->shininess = 200;

    auto wrist_material = material();
    *wrist_material = *sphere_material;
    wrist_material->color = color(0.1, 1, 1);

    auto palm_material = material();
    *palm_material = *sphere_material;
    palm_material->color = color(0.1, 0.1, 1);

    auto thumb_material = material();
    *thumb_material = *sphere_material;
    thumb_material->color = color(0.1, 0.1, 1);

    auto index_material = material();
    *index_material = *sphere_material;
    index_material->color = color(1, 1, 0.1);

    auto middle_material = material();
    *middle_material = *sphere_material;
    middle_material->color = color(0.1, 1, 0.5);

    auto ring_material = material();
    *ring_material = *sphere_material;
    ring_material->color = color(0.1, 1, 0.1);

    auto pinky_material = material();
    *pinky_material = *sphere_material;
    pinky_material->color = color(0.1, 0.5, 1);

    // ======================================================
    // a backdrop onto which to cast the shadow
    // ======================================================

    auto backdrop = sphere();
    backdrop->material->color = color(1, 1, 1);
    backdrop->material->ambient = 0;
    backdrop->material->diffuse = 0.5;
    backdrop->material->specular = 0;
    backdrop->transform = translation(0, 0, 20) * scaling(200, 200, 0.01);
    w->objects.push_back(backdrop);

    // ======================================================
    // describe the elements of the scene
    // ======================================================

    // the wrist
    auto wrist = sphere();
    wrist->material = wrist_material;
    wrist->transform = rotation_z(M_PI_4) * translation(-4, 0, -21) * scaling(3, 3, 3);
    w->objects.push_back(wrist);

    // the palm
    auto palm = sphere();
    palm->material = palm_material;
    palm->transform = translation(0, 0, -15) * scaling(4, 3, 3);
    w->objects.push_back(palm);

    // the thumb
    auto thumb = sphere();
    thumb->material = thumb_material;
    thumb->transform = translation(-2, 2, -16) * scaling(1, 3, 1);
    w->objects.push_back(thumb);

    // the index finger
    auto index_finger = sphere();
    index_finger->material = index_material;
    index_finger->transform = translation(3, 2, -22) * scaling(3, 0.75, 0.75);
    w->objects.push_back(index_finger);

    // the middle finger
    auto middle_finger = sphere();
    middle_finger->material = middle_material;
    middle_finger->transform = translation(4, 1, -19) * scaling(3, 0.75, 0.75);
    w->objects.push_back(middle_finger);

    // the ring finger
    auto ring_finger = sphere();
    ring_finger->material = ring_material;
    ring_finger->transform = translation(4, 0, -18) * scaling(3, 0.75, 0.75);
    w->objects.push_back(ring_finger);

    // the pinky finger
    auto pinky_finger = sphere();
    pinky_finger->material = pinky_material;
    pinky_finger->transform = translation(3, -1.5, -20) * rotation_z(-M_PI / 10) * translation(1, 0, 0) * scaling(2.5, 0.6, 0.6);
    w->objects.push_back(pinky_finger);

    auto image = render(cam, w);

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(image, png_file.string());

    return EXIT_SUCCESS;
}
