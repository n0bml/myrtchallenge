#pragma once

/**
 * @file include/myrtchallenge/materials.hpp
 * @brief Declaration of materials and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include <memory>

#include "myrtchallenge/colors.hpp"
#include "myrtchallenge/lights.hpp"
#include "myrtchallenge/patterns.hpp"

struct Shape;

struct Material
{
    Color color;
    double_t ambient;
    double_t diffuse;
    double_t specular;
    double_t shininess;
    Pattern_Ptr pattern;
    double_t reflective;
    double_t transparency;
    double_t refractive_index;

    bool operator==(const Material& rhs) const;
    bool operator!=(const Material& rhs) const;
};


using Material_Ptr = std::shared_ptr<Material>;


Material_Ptr material();


Color lighting(const Material_Ptr& material, const std::shared_ptr<Shape>& object, const Point_Light_Ptr& light, const Tuple& position, const Tuple& eyev, const Tuple& normalv, bool in_shadow);
