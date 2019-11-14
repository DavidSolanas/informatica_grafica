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
#include <memory>

std::array<std::unique_ptr<Geometry>, 17> scene1(Camera c, const int W, const int H)
{
    std::array<std::unique_ptr<Geometry>, 17> geometry;
    float split = W / 5;
    for (int i = 0; i < 5; i++)
    {
        geometry[i] = std::unique_ptr<Geometry>(new Sphere(Point((i + 1) * split - (split / 2), (H / 2 - 200) + i * 75, c.getF().mod() + i * 100), Direction(0, 200, 0),
                                                           Point((i + 1) * split - (split / 2) + 100, (H / 2 - 200) + i * 75, c.getF().mod() + i * 100)));
    }
    geometry[5] = std::unique_ptr<Geometry>(new Plane(Direction(0, 1, 0), Point(W / 2, c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod())));
    geometry[8] = std::unique_ptr<Geometry>(new Plane(Direction(0, -1, 0), Point(W / 2, c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod())));
    geometry[6] = std::unique_ptr<Geometry>(new Plane(Direction(1, 0, 0), Point(c.getO().getCoord()[0] - c.getL().mod(), H / 2, c.getF().mod())));
    geometry[7] = std::unique_ptr<Geometry>(new Plane(Direction(-1, 0, 0), Point(c.getO().getCoord()[0] + c.getL().mod(), H / 2, c.getF().mod())));
    geometry[9] = std::unique_ptr<Geometry>(new Plane(Direction(0, 0, -1), Point(W / 2, H / 2, c.getF().mod() + 750)));
    // CARA FRONTAL
    geometry[10] = std::unique_ptr<Geometry>(new BoundedPlane(Point(500, H - 100, c.getF().mod() + 300),
                                                              Point(800, H - 100, c.getF().mod() + 300),
                                                              Point(800, H - 300, c.getF().mod() + 300),
                                                              Point(500, H - 300, c.getF().mod() + 300)));

    // CARA DERECHA
    geometry[11] = std::unique_ptr<Geometry>(new BoundedPlane(Point(800, H - 100, c.getF().mod() + 300),
                                                              Point(900, H - 100, c.getF().mod() + 400),
                                                              Point(900, H - 300, c.getF().mod() + 400),
                                                              Point(800, H - 300, c.getF().mod() + 300)));
    //CARA IZQUIERDA
    geometry[12] = std::unique_ptr<Geometry>(new BoundedPlane(Point(500, H - 100, c.getF().mod() + 300),
                                                              Point(600, H - 100, c.getF().mod() + 400),
                                                              Point(600, H - 300, c.getF().mod() + 400),
                                                              Point(500, H - 300, c.getF().mod() + 300)));
    //CARA TRASERA
    geometry[13] = std::unique_ptr<Geometry>(new BoundedPlane(Point(600, H - 100, c.getF().mod() + 400),
                                                              Point(900, H - 100, c.getF().mod() + 400),
                                                              Point(900, H - 300, c.getF().mod() + 400),
                                                              Point(600, H - 300, c.getF().mod() + 400)));
    // CARA SUPERIOR
    geometry[14] = std::unique_ptr<Geometry>(new BoundedPlane(Point(500, H - 100, c.getF().mod() + 300),
                                                              Point(600, H - 100, c.getF().mod() + 400),
                                                              Point(900, H - 100, c.getF().mod() + 400),
                                                              Point(800, H - 100, c.getF().mod() + 300)));
    //CARA INFERIOR
    geometry[15] = std::unique_ptr<Geometry>(new BoundedPlane(Point(500, H - 300, c.getF().mod() + 300),
                                                              Point(600, H - 300, c.getF().mod() + 400),
                                                              Point(900, H - 300, c.getF().mod() + 400),
                                                              Point(800, H - 300, c.getF().mod() + 300)));

    geometry[16] = std::unique_ptr<Geometry>(new Triangle(Point(500, H - 100, c.getF().mod() + 299),
                                                          Point(800, H - 300, c.getF().mod() + 299),
                                                          Point(500, H - 300, c.getF().mod() + 299)));
    return geometry;
}

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

    std::array<std::unique_ptr<Geometry>, 17> geometry = scene1(c, W, H);

    Point light(W / 2, H - 100, c.getF().mod() + 250);
    float power = 1200000;

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
                    for (int j = 0; j < geometry.size(); j++)
                    {
                        if (geometry[j]->intersect(c.getO(), d_ray, t))
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
                        Direction normal = geometry[i]->getNormal(X);
                        Direction wi = normalize(light - X);
                        Direction wo = normalize(c.getO() - X);
                        float kd = i == 5 ? 0.9f : 0.6f;
                        float ks = i == 5 ? 0.05f : 0.25f;
                        float brdf = phong_BRDF(kd, ks, 10, normal, wi, wo);
                        float geometry = dot(normal, wi) < 0 ? 0 : dot(normal, wi);
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
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        color.setR(255);
                        color.setG(0);
                        color.setB(0);
                        break;
                    case 10:
                        color.setR(255);
                        color.setG(255);
                        color.setB(0);
                        break;
                    case 11:
                        color.setR(0);
                        color.setG(0);
                        color.setB(255);
                        break;
                    case 14:
                        color.setR(0);
                        color.setG(255);
                        color.setB(0);
                        break;

                    default:
                        color.setR(141);
                        color.setG(141);
                        color.setB(141);
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