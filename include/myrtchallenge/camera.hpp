#pragma once

/**
 * @file include/myrtchallenge/camera.hpp
 * @brief Declaration of camera and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include <memory>

#include "myrtchallenge/canvas.hpp"
#include "myrtchallenge/matrices.hpp"
#include "myrtchallenge/world.hpp"


struct Camera
{
    double_t hsize;
    double_t vsize;
    double_t field_of_view;
    Matrix transform;

    double_t pixel_size;
    double_t half_width;
    double_t half_height;
};


using Camera_Ptr = std::unique_ptr<Camera>;


Camera_Ptr camera(double_t hsize, double_t vsize, double_t field_of_view);
Ray ray_for_pixel(const Camera_Ptr& camera, double_t px, double_t py);
Canvas render(const Camera_Ptr& camera, const World_Ptr& world);
