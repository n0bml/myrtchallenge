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


struct Material
{
    Color color;
    double_t ambient;
    double_t diffuse;
    double_t specular;
    double_t shininess;

    bool operator==(const Material& rhs) const;
};


using Material_Ptr = std::shared_ptr<Material>;


Material_Ptr material();


Color lighting(const Material_Ptr material, const Point_Light_Ptr light, const Tuple& position, const Tuple& eyev, const Tuple& normalv);
