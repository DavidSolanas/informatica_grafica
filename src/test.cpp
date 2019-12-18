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

    Sphere s(Point(200, 200, 70), Direction(0, 50, 0), Point(175, 200, 70));
    Camera c(Direction(0, 0, 20), Direction((int)H / 2, 0, 0), Direction(0, (int)W / 2, 0), Point(200, 200, 0));
    //Point c(200, 300, 0);
    Plane p(Direction(-1, -1, -1), Point(200, 200, 20));

    std::ofstream f("/Users/santi/Desktop/prueba.ppm");
    if (f.is_open())
    {
        f << "P3" << std::endl;
        f << W << " " << H << std::endl;
        f << "255" << std::endl;
        int inicioX = c.getO().getCoord()[0] - c.getL().mod();
        int finX = c.getO().getCoord()[0] + c.getL().mod();
        int inicioY = c.getO().getCoord()[1] - c.getU().mod();
        int finY = c.getO().getCoord()[1] + c.getU().mod();
        float xmin = 200, xmax = 200;
        float ymin = 200, ymax = 200;
        float zmin = 70, zmax = 70;
        for (int y = finY; y > inicioY; y--)
        {
            for (int x = inicioX; x < finX; x++)
            {
                Point pixel(x, y, 20);
                Direction d_ray = normalize(pixel - c.getO());
                float t;
                if (p.intersect(pixel, d_ray, t))
                {
                    Point P = c.getO() + d_ray * t;
                    auto cc = P.getCoord();
                    if (cc[0] > xmax)
                        xmax = cc[0];
                    if (cc[0] < xmin)
                        xmin = cc[0];
                    if (cc[1] > ymax)
                        ymax = cc[1];
                    if (cc[1] < ymin)
                        ymin = cc[1];
                    if (cc[2] > zmax)
                        zmax = cc[2];
                    if (cc[2] < zmin)
                        zmin = cc[2];
                    //std::cout << "<" << cc[0] << ", " << cc[1] << ", " << cc[2] << ">" << std::endl;
                    f << "255 0 0\t";
                }
                else
                    f << "255 255 255\t";
            }
            f << "\n";
        }
        std::cout << "X: " << xmin << " - " << xmax << std::endl;
        std::cout << "Y: " << ymin << " - " << ymax << std::endl;
        std::cout << "Z: " << zmin << " - " << zmax << std::endl;
    }
    return 0;
}