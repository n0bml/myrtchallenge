/**
 * @file src/intersections.cpp
 * @brief Definition of intersections and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <algorithm>
#include <cmath>

#include "myrtchallenge/intersections.hpp"
#include "myrtchallenge/spheres.hpp"
#include "myrtchallenge/tuples.hpp"

#include "primitives.hpp"


bool Intersection::operator<(const Intersection& rhs) const
{
    return t < rhs.t;
}


bool Intersection::operator==(const Intersection& rhs) const
{
    return equal(t, rhs.t) && object == rhs.object;
}


Intersection hit(const Intersections& is)
{
    auto i = std::find_if(std::begin(is), std::end(is), [](const Intersection& i){ return i.t > 0; });
    if (i == std::end(is))
        return intersection(NAN, nullptr);
    return *i;
}


Intersection intersection(double_t t, Sphere_Ptr object)
{
    return Intersection{t, object};
}


Intersections intersections(const Intersections& is)
{
    Intersections results{is};
    std::sort(std::begin(results), std::end(results));
    return results;
}


Computations prepare_computations(const Intersection& i, const Ray& ray)
{
    Computations comps;

    // copy the intersection's properties, for convenience
    comps.t      = i.t;
    comps.object = i.object;

    // precompute some useful values
    comps.point   = position(ray, comps.t);
    comps.eyev    = -ray.direction;
    comps.normalv = normal_at(comps.object, comps.point);

    if (dot(comps.normalv, comps.eyev) < 0) {
        comps.inside  = true;
        comps.normalv = -comps.normalv;
    }
    else
        comps.inside = false;

    comps.over_point = comps.point + comps.normalv * EPSILON;

    return comps;
}
