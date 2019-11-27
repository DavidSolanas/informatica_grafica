/****************************************+
 * Fichero: Path_tracer.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include <iostream>
#include "Geometry.hpp"
#include <cmath>
#include "Scene.hpp"
#include "Light.hpp"

int main(int argc, char const *argv[])
{

    std::string usage = "Usage: Path_tracer -n ppp";
    if (argc != 3)
    {
        std::cerr << "Incorrect number of parameters" << std::endl;
        std::cerr << usage << std::endl;
        exit(1);
    }
    else
    {
        int ppp = 1;
        for (int i = 1; i < argc; i++)
        {
            switch (i)
            {
            case 1:
                if (strcmp("-n", argv[i]) != 0)
                {
                    std::cerr << "Incorrect parameter, found \"" << argv[i] << "\" but expected \"-n\"" << std::endl;
                    std::cerr << usage << std::endl;
                    exit(1);
                }
                break;
            case 2:
                ppp = atoi(argv[i]);
                break;

            default:
                break;
            }
        }
        const int H = 1080, W = 1920;

        Direction l((int)W / 2, 0, 0);
        Direction u(0, (int)H / 2, 0);
        Direction f(0, 0, u.mod() / tan(M_PI / 24));
        Point c0((int)W / 2, (int)H / 2, 0);
        Camera c(f, u, l, c0);
        //ray_tracer("/Users/david/Desktop/prueba.ppm", ppp, c, W, H);
    }
    return 0;
}
