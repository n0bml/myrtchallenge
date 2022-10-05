#pragma once

/**
 * @file include/myrtchallenge/patterns.hpp
 * @brief Declaration of patterns and support functions.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cmath>
#include <memory>

#include "myrtchallenge/colors.hpp"
#include "myrtchallenge/matrices.hpp"
#include "myrtchallenge/tuples.hpp"


struct Shape;


struct Pattern : std::enable_shared_from_this<Pattern>
{
    virtual ~Pattern() = default;

    Color a;
    Color b;
    Matrix transform;

    virtual Color pattern_at(const Tuple& point) const = 0;
};


struct Checkers_Pattern : public Pattern
{
    Color pattern_at(const Tuple& point) const;
};


struct Gradient_Pattern : public Pattern
{
    Color pattern_at(const Tuple& point) const;
};


struct Ring_Pattern : public Pattern
{
    Color pattern_at(const Tuple& point) const;
};


struct Stripe_Pattern : public Pattern
{
    Color pattern_at(const Tuple& point) const;
};


using Pattern_Ptr = std::shared_ptr<Pattern>;

Pattern_Ptr checkers_pattern(const Color& first, const Color& second);
Pattern_Ptr gradient_pattern(const Color& first, const Color& second);
Pattern_Ptr ring_pattern(const Color& first, const Color& second);
Pattern_Ptr stripe_pattern(const Color& first, const Color& second);


Color pattern_at_shape(const Pattern_Ptr& pattern, const std::shared_ptr<Shape>& object, const Tuple& world_point);
void set_pattern_transform(Pattern_Ptr& pattern, const Matrix& m);
