/**
 * @file examples/ticker.cpp
 * @brief Example program showing how to use tuples.
 * @author Brendan Leber <brendan@brendanleber.com>
 * @copyright Copyright 2022 by Brendan Leber.  Some rights reserved, see LICENSE.
 */

#include <cstdio>
#include <cstdlib>

#include "myrtchallenge/tuples.hpp"


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
    // projectile starts one unit above the origin.
    // velocity is normalized to 1 unit/tick.
    auto p = Projectile{point(0, 1, 0), normalize(vector(1, 1, 0))};

    // gravity is -0.1 unit/tick, and wind is -0.01 unit/tick
    auto e = Environment{vector(0, -0.1, 0), vector(-0.01, 0, 0)};

    printf("tick, x, y, z, w\n");
    int ticks = 0;
    do {
        printf("%d, %f, %f, %f, %f\n", ticks++, p.position.x, p.position.y, p.position.z, p.position.w);
        p = tick(e, p);
    } while (p.position.y >= 0);
    printf("%d, %f, %f, %f, %f\n", ticks, p.position.x, p.position.y, p.position.z, p.position.w);

    return EXIT_SUCCESS;
}
