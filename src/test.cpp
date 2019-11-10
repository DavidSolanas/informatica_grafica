#include <iostream>
#include <cmath>
#include <fstream>
#include "Transformation.hpp"
#include "Sphere.hpp"
#include "Scene.hpp"
#include "Plane.hpp"
#include "Ray_tracer.hpp"

int main(int argc, const char **argv)
{
    const int H = 1080, W = 1920;

    Direction l((int)W / 2, 0, 0);
    Direction u(0, (int)H / 2, 0);
    Direction f(0, 0, u.mod() / tan(M_PI / 24));
    Point c0((int)W / 2, (int)H / 2, 0);
    Camera c(f, u, l, c0);
    ray_tracer("/Users/david/Desktop/prueba.ppm", c, W, H);
    return 0;
}