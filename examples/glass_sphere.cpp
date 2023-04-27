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

    // - add: camera
    //   width: 300
    //   height: 300
    //   field-of-view: 0.45
    //   from: [ 0, 0, -5 ]
    //   to: [ 0, 0, 0 ]
    //   up: [ 0, 1, 0 ]
    auto cam = camera(300, 300, 0.45);
    cam->transform = view_transform(point(0, 0, -5), point(0, 0, 0), vector(0, 1, 0));

    // - add: light
    //   intensity: [ 0.9, 0.9, 0.9 ]
    //   at: [ 2, 10, -5 ]
    w->light = point_light(point(2, 10, -5), color(0.9, 0.9, 0.9));

    // # wall
    // - add: plane
    //   transform:
    //     - [ rotate-x, 1.5708 ]
    //     - [ translate, 0, 0, 10 ]
    //   material:
    //     pattern:
    //       type: checkers
    //       colors:
    //         - [ 0.15, 0.15, 0.15 ]
    //         - [ 0.85, 0.85, 0.85 ]
    //     ambient: 0.8
    //     diffuse: 0.2
    //     specular: 0
    auto wall = plane();
    wall->transform = translation(0, 0, 10) * rotation_x(1.5708);
    wall->material->pattern  = checkers_pattern(color(0.15, 0.15, 0.15), color(0.85, 0.85, 0.85));
    wall->material->ambient  = 0.8;
    wall->material->diffuse  = 0.2;
    wall->material->specular = 0;
    w->objects.push_back(wall);

    // # glass ball
    // - add: sphere
    //   material:
    //     color: [ 1, 1, 1 ]
    //     ambient: 0
    //     diffuse: 0
    //     specular: 0.9
    //     shininess: 300
    //     reflective: 0.9
    //     transparency: 0.9
    //     refractive-index: 1.5
    auto glass = sphere();
    glass->material->color            = color(1, 1, 1);
    glass->material->ambient          = 0;
    glass->material->diffuse          = 0;
    glass->material->specular         = 0.9;
    glass->material->shininess        = 300;
    glass->material->reflective       = 0.9;
    glass->material->transparency     = 0.9;
    glass->material->refractive_index = 1.5;
    w->objects.push_back(glass);

    // # hollow center
    // - add: sphere
    //   transform:
    //     - [ scale, 0.5, 0.5, 0.5 ]
    //   material:
    //     color: [ 1, 1, 1 ]
    //     ambient: 0
    //     diffuse: 0
    //     specular: 0.9
    //     shininess: 300
    //     reflective: 0.9
    //     transparency: 0.9
    //     refractive-index: 1.0000034
    auto hollow = sphere();
    hollow->transform = scaling(0.5, 0.5, 0.5);
    hollow->material->color            = color(1, 1, 1);
    hollow->material->ambient          = 0;
    hollow->material->diffuse          = 0;
    hollow->material->specular         = 0.9;
    hollow->material->shininess        = 300;
    hollow->material->reflective       = 0.9;
    hollow->material->transparency     = 0.9;
    hollow->material->refractive_index = 1.0000034;
    w->objects.push_back(hollow);

    auto image = render(cam, w);

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(image, png_file.string());

    return EXIT_SUCCESS;
}
