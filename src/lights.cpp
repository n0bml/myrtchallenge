/**
 * @file src/lights.cpp
 * @brief Definition of lights and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>

#include "myrtchallenge/lights.hpp"

#include "primitives.hpp"


#if 0
Point_Light& Point_Light::operator=(const Point_Light& other)
{
    if (this != &other) {
        position  = other.position;
        intensity = other.intensity;
    }
    return *this;
}
#endif


bool Point_Light::operator==(const Point_Light& rhs) const
{
    if (position != rhs.position)
        return false;
    if (intensity != rhs.intensity)
        return false;
    return true;
}


Point_Light_Ptr point_light(const Tuple& position, const Color& intensity)
{
    return std::make_shared<Point_Light>(position, intensity);
}
