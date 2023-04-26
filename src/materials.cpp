/**
 * @file src/materials.cpp
 * @brief Definition of materials and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */


#include <cassert>
#include <cmath>
#include <optional>

#include "myrtchallenge/patterns.hpp"
#include "myrtchallenge/materials.hpp"
#include "myrtchallenge/shapes.hpp"

#include "primitives.hpp"


bool Material::operator==(const Material& rhs) const
{
    if (!(color == rhs.color)) {
        return false;
    }
    if (!equal(ambient, rhs.ambient)) {
        return false;
    }
    if (!equal(diffuse, rhs.diffuse)) {
        return false;
    }
    if (!equal(specular, rhs.specular)) {
        return false;
    }
    if (!equal(shininess, rhs.shininess)) {
        return false;
    }
    if (!equal(transparency, rhs.transparency)) {
        return false;
    }
    if (!equal(refractive_index, rhs.refractive_index)) {
        return false;
    }
    return true;
}


bool Material::operator!=(const Material& rhs) const
{
    return !(*this == rhs);
}


Material_Ptr material()
{
    auto ptr = std::make_shared<Material>();
    ptr->color            = color(1, 1, 1);
    ptr->ambient          = 0.1;
    ptr->diffuse          = 0.9;
    ptr->specular         = 0.9;
    ptr->shininess        = 200.0;
    ptr->pattern          = nullptr;
    ptr->reflective       = 0.0;
    ptr->transparency     = 0.0;
    ptr->refractive_index = 1.0;
    return ptr;
}


Color lighting(const Material_Ptr& material, const Shape_Ptr& object, const Point_Light_Ptr& light, const Tuple& point, const Tuple& eyev, const Tuple& normalv, bool in_shadow)
{
    auto effective_color = material->color;
    if (material->pattern)
        effective_color = pattern_at_shape(material->pattern, object, point);

    // combine the surface color with the light's color/itensity.
    effective_color = effective_color * light->intensity;

    // find the direction of the light source
    auto lightv = normalize(light->position - point);

    // compute the ambient contribution
    auto ambient = effective_color * material->ambient;

    Color diffuse, specular;

    // light_dot_normal represents the cosine of the angle between the
    // light vector and the normal vector.  a negative number means the
    // light is on the other side of the surface.
    auto light_dot_normal = dot(lightv, normalv);
    if (light_dot_normal < 0) {
        diffuse = color(0, 0, 0);
        specular = color(0, 0, 0);
    }
    else {
        // compute the diffuse contribution
        diffuse = effective_color * material->diffuse * light_dot_normal;

        // reflect_dot_eye represents the cosine of the angle between the
        // reflection vector and the eye vector.  a negative number means the
        // light reflects away from the eye.
        auto reflectv = reflect(-lightv, normalv);
        auto reflect_dot_eye = dot(reflectv, eyev);
        if (reflect_dot_eye <= 0) {
            specular = color(0, 0, 0);
        }
        else {
            // compute the specular contribution
            auto factor = std::pow(reflect_dot_eye, material->shininess);
            specular = light->intensity * material->specular * factor;
        }
    }

    // add the three contributions together to get the final shading
    if (in_shadow)
        return ambient;
    return ambient + diffuse + specular;
}
