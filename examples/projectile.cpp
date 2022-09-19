/**
 * @file examples/projectile.cpp
 * @brief Example program showing how to use the canvas save an image.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>

#include "myrtchallenge/canvas.hpp"
#include "myrtchallenge/tuples.hpp"


namespace fs = std::filesystem;


struct Projectile
{
    Tuple position;
    Tuple velocity;

    Projectile(const Tuple& p, const Tuple& v) : position{p}, velocity{v} {}
};


struct Environment
{
    Tuple gravity;
    Tuple wind;

    Environment(const Tuple& g, const Tuple& w) : gravity{g}, wind{w} {}
};


Projectile tick(const Environment& env, const Projectile& proj)
{
    auto position = proj.position + proj.velocity;
    auto velocity = proj.velocity + env.gravity + env.wind;
    return Projectile{position, velocity};
}


int main(int argc, char* argv[], char* envp[])
{
    auto start = point(0, 1, 0);
    auto velocity = normalize(vector(1, 1.8, 0)) * 11.25;
    auto p = Projectile{start, velocity};

    auto gravity = vector(0, -0.1, 0);
    auto wind = vector(-0.01, 0, 0);
    auto e = Environment{gravity, wind};

    auto c = canvas(900, 550);

    do {
        auto x = static_cast<size_t>(std::ceil(p.position.x));
        if (x < 0 || x >= c.width)
            continue;

        auto y = static_cast<size_t>(std::ceil(p.position.y));
        if (y < 0 || y >= c.height)
            continue;

        write_pixel(c, x, c.height - y, color(1, 1, 1));
        p = tick(e, p);
    } while (p.position.y >= 0 && p.position.x <= c.width);

    fs::path png_file{argv[0]};
    png_file.replace_extension("png");
    canvas_to_png(c, png_file.string());

    return EXIT_SUCCESS;
}
