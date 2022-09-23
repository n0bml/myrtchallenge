#pragma once

/**
 * @file include/myrtchallenge/lights.hpp
 * @brief Declaration of lights and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <memory>

#include "myrtchallenge/colors.hpp"
#include "myrtchallenge/tuples.hpp"


struct Point_Light
{
    Tuple position;
    Color intensity;

    //Point_Light& operator=(const Point_Light& other);
    //bool operator==(const Point_Light& rhs) const;
};


using Point_Light_Ptr = std::shared_ptr<Point_Light>;


Point_Light_Ptr point_light(const Tuple& position, const Color& intensity);
