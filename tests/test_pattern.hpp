/**
 * @file tests/test_pattern.hpp
 * @brief A test pattern to be used testing the abstract base class Pattern.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <memory>

#include <myrtchallenge/colors.hpp>
#include <myrtchallenge/patterns.hpp>
#include <myrtchallenge/tuples.hpp>


struct Test_Pattern : public Pattern
{
    Color pattern_at(const Tuple& point) const
    {
        return color(point.x, point.y, point.z);
    }
};


Pattern_Ptr test_pattern()
{
    auto ptr = std::make_shared<Test_Pattern>();
    ptr->a         = color(1, 1, 1);
    ptr->b         = color(0, 0, 0);
    ptr->transform = identity_matrix();
    return ptr;
}
