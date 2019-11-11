/****************************************+
 * Fichero: Ray_tracer.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Ray_tracer.hpp"
#include <cmath>
#include "Sphere.hpp"
#include "Plane.hpp"
#include "RGB.hpp"
#include <iostream>
#include <random>

float phong_BRDF(const float kd, const float ks, const float alpha, Direction n,
                 Direction wi, Direction wo)
{
    Direction wr = wi - (wi - (n * dot(wi, n))) * 2;
    return (kd / M_PI) + ((ks * (alpha + 2) / (2 * M_PI)) * pow(abs(dot(wr, wo)), alpha));
}

float lambertian_BRDF(const float kd)
{
    return (kd / M_PI);
}

void ray_tracer(std::string filename, const int n_ray, Camera c, const int W, const int H)
{
    //Random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    std::array<Sphere, 5> spheres;
    float split = W / 5;
    for (int i = 0; i < 5; i++)
    {
        Sphere s(Point((i + 1) * split - (split / 2), H / 2, c.getF().mod()), Direction(0, 200, 0),
                 Point((i + 1) * split - (split / 2) + 100, H / 2, c.getF().mod()));
        spheres[i] = s;
    }
    //Plane p(Direction(-1, -1, -1), Point(200, 200, 20));
    Point light(W / 2, H - 300, c.getF().mod() - 900);
    float power = 2000000;

    std::ofstream _f(filename);
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
                float Lo = 0;
                int i;
                for (int k = 0; k < n_ray; k++)
                {
                    i = -1;
                    float xrand = dist(mt), yrand = dist(mt);
                    Point pixel(x + xrand, y + yrand, c.getF().mod());
                    Direction d_ray = normalize(pixel - c.getO());
                    float t, tmin = INFINITY;
                    for (int j = 0; j < spheres.size(); j++)
                    {
                        if (spheres[j].intersect(c.getO(), d_ray, t))
                        {
                            if (t > 0 && t < tmin)
                            {
                                tmin = t;
                                i = j;
                            }
                        }
                    }
                    if (i != -1)
                    {
                        // Calcular luz
                        Point X = c.getO() + (d_ray * tmin);
                        float Li = power / ((light - X).mod() * (light - X).mod());
                        Direction normal = normalize(X - spheres[i].getCenter());
                        Direction wi = normalize(light - X);
                        Direction wo = normalize(c.getO() - X);
                        float brdf = phong_BRDF(0.8f, 0.1f, 2, normal, wi, wo);
                        float geometry = abs(dot(normal, wi));
                        Lo += Li * brdf * geometry;
                    }
                }
                if (i != -1)
                {
                    RGB color;
                    switch (i)
                    {
                    case 0:
                        color.setR(50);
                        color.setG(205);
                        color.setB(50);
                        break;
                    case 1:
                        color.setR(0);
                        color.setG(255);
                        color.setB(255);
                        break;
                    case 2:
                        color.setR(255);
                        color.setG(0);
                        color.setB(255);
                        break;
                    case 3:
                        color.setR(208);
                        color.setG(208);
                        color.setB(208);
                        break;
                    case 4:
                        color.setR(255);
                        color.setG(137);
                        color.setB(11);
                        break;

                    default:
                        break;
                    }
                    Lo /= n_ray;
                    _f << Lo * color.getR() << " "
                       << Lo * color.getG() << " "
                       << Lo * color.getB() << "\t";
                }
                else
                {
                    _f << 255 << " " << 255 << " " << 255 << "\t";
                }
            }
            _f << "\n";
        }
    }
    _f.close();
}

int main(int argc, const char **argv)
{

    std::string usage = "Usage: Ray_tracer -n n_rays";
    if (argc != 3)
    {
        std::cerr << "Incorrect number of parameters" << std::endl;
        std::cerr << usage << std::endl;
        exit(1);
    }
    else
    {
        int n_ray = 1;
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
                n_ray = atoi(argv[i]);
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
        ray_tracer("/Users/david/Desktop/prueba.ppm", n_ray, c, W, H);
    }
    return 0;
}