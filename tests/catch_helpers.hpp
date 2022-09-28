/**
 * @file tests/catch_helpers.cpp
 * @brief Helper functions for unit testing with Catch2.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <iomanip>
#include <ostream>

#include <myrtchallenge/colors.hpp>
#include <myrtchallenge/matrices.hpp>
//#include <myrtchallenge/spheres.hpp>
#include <myrtchallenge/tuples.hpp>


inline std::ostream& operator<<(std::ostream& os, const Tuple& value)
{
    if (value.is_point())
        os << "point(";
    else
        os << "vector(";
    os << value.x << ", " << value.y << ", " << value.z << ')';
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const Color& color)
{
    os << "color(" << color.red << ", " << color.green << ", " << color.blue << ')';
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    os
        <<  "| " << std::setw(8) << std::setprecision(5) << m(0, 0)
        << " | " << std::setw(8) << std::setprecision(5) << m(0, 1)
        << " | " << std::setw(8) << std::setprecision(5) << m(0, 2)
        << " | " << std::setw(8) << std::setprecision(5) << m(0, 3)
        << "|\n";

    os
        <<  "| " << std::setw(8) << std::setprecision(5) << m(1, 0)
        << " | " << std::setw(8) << std::setprecision(5) << m(1, 1)
        << " | " << std::setw(8) << std::setprecision(5) << m(1, 2)
        << " | " << std::setw(8) << std::setprecision(5) << m(1, 3)
        << "|\n";

    os
        <<  "| " << std::setw(8) << std::setprecision(5) << m(2, 0)
        << " | " << std::setw(8) << std::setprecision(5) << m(2, 1)
        << " | " << std::setw(8) << std::setprecision(5) << m(2, 2)
        << " | " << std::setw(8) << std::setprecision(5) << m(2, 3)
        << "|\n";

    os
        <<  "| " << std::setw(8) << std::setprecision(5) << m(3, 0)
        << " | " << std::setw(8) << std::setprecision(5) << m(3, 1)
        << " | " << std::setw(8) << std::setprecision(5) << m(3, 2)
        << " | " << std::setw(8) << std::setprecision(5) << m(3, 3)
        << "|\n";

    return os;
}

#if 0
inline std::ostream& operator<<(std::ostream& os, const Material& material)
{
    return os
        << "material:\n"
        << "  color: " << material.color << '\n'
        << "  ambient: " << material.ambient << '\n'
        << "  diffuse: " << material.diffuse << '\n'
        << "  specular: " << material.specular << '\n'
        << "  shininess: " << material.shininess << '\n';
}


inline std::ostream& operator<<(std::ostream& os, const Sphere& sphere)
{
    return os
        << "sphere:\n"
        << "  matrix:  tbd\n"
        << sphere.material << '\n';
}
#endif
