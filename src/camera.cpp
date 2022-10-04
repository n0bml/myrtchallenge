/**
 * @file src/camera.cpp
 * @brief Definition of camera and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include "myrtchallenge/camera.hpp"
#include "myrtchallenge/transformations.hpp"

#include "primitives.hpp"


Camera_Ptr camera(double_t hsize, double_t vsize, double_t field_of_view)
{
    auto cam = std::make_unique<Camera>(hsize, vsize, field_of_view, identity_matrix());

    auto half_view = std::tan(cam->field_of_view / 2);
    auto aspect = cam->hsize / cam->vsize;
    if (aspect >= 1) {
        cam->half_width = half_view;
        cam->half_height = half_view / aspect;
    }
    else {
        cam->half_width = half_view * aspect;
        cam->half_height = half_view;
    }

    cam->pixel_size = (cam->half_width * 2) / cam->hsize;

    return cam;
}


Ray ray_for_pixel(const Camera_Ptr& camera, double_t px, double_t py)
{
    // the offset from the edge of the canvas to the pixel's center
    auto xoffset = (px + 0.5) * camera->pixel_size;
    auto yoffset = (py + 0.5) * camera->pixel_size;

    // the untransformed coordinates of the pixel in world space
    // (remember that the camera looks towards -z, so +x is to the "left".)
    auto world_x = camera->half_width - xoffset;
    auto world_y = camera->half_height - yoffset;

    // using the camera matrix, transform the canvas point and the origin,
    // and then compute the ray's direction vector.
    // (remember that the canvas is at z = -1)
    auto pixel = inverse(camera->transform) * point(world_x, world_y, -1);
    auto origin = inverse(camera->transform) * point(0, 0, 0);
    auto direction = normalize(pixel - origin);

    return ray(origin, direction);
}


Canvas render(const Camera_Ptr& camera, const World_Ptr& world)
{
    auto image = canvas(static_cast<size_t>(camera->hsize), static_cast<size_t>(camera->vsize));
    for (double_t y = 0; y < camera->vsize; ++y) {
        for (double_t x = 0; x < camera->hsize; ++x) {
            auto ray = ray_for_pixel(camera, x, y);
            auto color = color_at(world, ray);
            write_pixel(image, x, y, color);
        }
    }
    return image;
}
