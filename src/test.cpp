#include <iostream>
#include <cmath>
#include <fstream>
#include "Transformation.hpp"
#include "Sphere.hpp"
#include "Tone_mapping.hpp"
#include "Scene.hpp"
#include "Plane.hpp"

int main(int argc, const char **argv)
{
    const int H = 400, W = 400;
    //Image img = load_HDR_image("../media/hdr-ppm/mpi_atrium_1.ppm");
    //local_reinhard(img, 1, 43, 8.0, 0.18, 0.05, 8);
    //save_LDR_image("/Users/david/Desktop/ldr_mpi_atrium_1_local.ppm", 65535, img);

    Direction l((int)H / 2, 0, 0);
    Direction u(0, (int)W / 2, 0);
    Direction f(0, 0, u.mod() / tan(M_PI / 6));
    Point c0(200, 200, 0);
    Camera c(f, u, l, c0);

    Sphere s(Point(100, 200, 347), Direction(0, 50, 0), Point(125, 200, 347));
    Plane p(Direction(-1, -1, -1), Point(200, 200, 20));

    std::ofstream _f("/Users/david/Desktop/prueba.ppm");
    if (_f.is_open())
    {
        _f << "P3" << std::endl;
        _f << W << " " << H << std::endl;
        _f << "255" << std::endl;
        int inicioX = c.getO().getCoord()[0] - c.getL().mod();
        int finX = c.getO().getCoord()[0] + c.getL().mod();
        int inicioY = c.getO().getCoord()[1] - c.getU().mod();
        int finY = c.getO().getCoord()[1] + c.getU().mod();
        for (int y = finY; y > inicioY; y--)
        {
            for (int x = inicioX; x < finX; x++)
            {
                Point pixel(x, y, 200 / tan(M_PI / 6));
                Direction d_ray = normalize(pixel - c.getO());
                float t;
                if (s.intersect(c.getO(), d_ray, t))
                {
                    //std::cout << "<" << cc[0] << ", " << cc[1] << ", " << cc[2] << ">" << std::endl;
                    _f << "255 0 0\t";
                }
                else
                    _f << "255 255 255\t";
            }
            _f << "\n";
        }
    }
    return 0;
}