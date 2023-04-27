/**
 * @file src/intersections.cpp
 * @brief Definition of intersections and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <algorithm>
#include <cmath>

#include "myrtchallenge/intersections.hpp"
#include "myrtchallenge/shapes.hpp"
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


Intersection intersection(double_t t, Shape_Ptr object)
{
    return Intersection{t, object};
}


Intersections intersections(const Intersections& is)
{
    Intersections results{is};
    std::sort(std::begin(results), std::end(results));
    return results;
}


Computations prepare_computations(const Intersection& i, const Ray& ray, const Intersections& xs)
{
    Computations comps;

    std::vector<Shape_Ptr> containers;
    for (auto it = std::begin(xs); it != std::end(xs); ++it) {
        if (*it == i) {
            if (containers.empty())
                comps.n1 = 1.0;
            else
                comps.n1 = containers.back()->material->refractive_index;
        }

        auto cit = std::find(std::begin(containers), std::end(containers), it->object);
        if (cit != std::end(containers))
            containers.erase(cit);
        else
            containers.push_back(it->object);

        if (*it == i) {
            if (containers.empty())
                comps.n2 = 1.0;
            else
                comps.n2 = containers.back()->material->refractive_index;
        }
    }

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

    comps.reflectv    = reflect(ray.direction, comps.normalv);
    comps.over_point  = comps.point + comps.normalv * EPSILON;
    comps.under_point = comps.point - comps.normalv * EPSILON;

    return comps;
}


double_t schlick(const Computations& comps)
{
    // find the cosine of the angle between the eye and normal vectors
    auto cos = dot(comps.eyev, comps.normalv);

    // total internal reflection can only occurs if n1 > n2
    if (comps.n1 > comps.n2) {
        auto n = comps.n1 / comps.n2;
        auto sin2_t = std::pow(n, 2) * (1.0 - std::pow(cos, 2));
        if (sin2_t > 1.0)
            return 1.0;

        // compute cosine of theta_t using trig identity
        auto cos_t = std::sqrt(1.0 - sin2_t);

        // when n1 > n2, use cos(theta_t) instead
        cos = cos_t;
    }

    auto r0 = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);

    return r0 + (1 - r0) * std::pow(1 - cos, 5);
}
