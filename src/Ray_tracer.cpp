/****************************************+
 * Fichero: Ray_tracer.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Ray_tracer.hpp"
#include "BRDF.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Textures.hpp"
#include "Transformation.hpp"
#include "Light.hpp"
#include <iostream>
#include <random>
#include <memory>
#include <cstring>

void ray_tracer(std::string filename, const int n_ray, Camera c, const int W, const int H)
{
    //Random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    std::vector<std::vector<RGB>> data = load_texture("/Users/david/Desktop/earthmap1k.ppm");

    std::array<std::unique_ptr<Geometry>, 7> geometry = scene3(c, W, H);

    float power = 3600000;
    PointLight light(Point(W / 2, H - 200, c.getF().mod() - 500), power, RGB(255, 255, 255));

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
                Point X;
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
                        // Calcular luz puntual
                        X = c.getO() + (d_ray * tmin);
                        float Li = power / ((light.p - X).mod() * (light.p - X).mod());
                        Direction normal = geometry[i]->getNormal(X);
                        Direction wi = normalize(light.p - X);
                        Direction wo = normalize(c.getO() - X);
                        float kd = i == 5 ? 0.9f : 0.6f;
                        float ks = i == 5 ? 0.05f : 0.25f;
                        float brdf = phong_BRDF(kd, ks, 10, normal, wi, wo);
                        float g = dot(normal, wi) < 0 ? 0 : dot(normal, wi);
                        Lo += Li * brdf * g;
                    }
                }
                if (i != -1)
                {
                    RGB color;
                    Sphere s;
                    Cylinder cy;
                    Triangle t;
                    BoundedPlane bp;
                    Cone cone;
                    Direction normal;
                    float u, v;
                    switch (i)
                    {
                    case 0:
                        color.setR(240);
                        color.setG(240);
                        color.setB(240);
                        break;
                    case 1:
                        color.setR(240);
                        color.setG(240);
                        color.setB(240);
                        break;
                    case 2:
                        bp = *reinterpret_cast<BoundedPlane *>((geometry[i].get()));
                        bp.get_uv(X, u, v);
                        color = get_pixel(data, u, v);
                        break;
                    case 3:
                        color.setR(240);
                        color.setG(240);
                        color.setB(240);
                        break;
                    case 4:
                        color.setR(240);
                        color.setG(240);
                        color.setB(240);
                        break;
                    case 5:
                        normal = geometry[i]->getNormal(X);
                        cy = *reinterpret_cast<Cylinder *>((geometry[i].get()));
                        cy.get_uv(normal, X.getCoord()[1] - cy.get_base_Y_coord(), u, v);
                        color = get_pixel(data, u, v);
                        break;
                    case 6:
                        s = *reinterpret_cast<Sphere *>((geometry[i].get()));
                        normal = normalize(s.getCenter() - X);
                        s.get_uv(normal, u, v);
                        color = get_pixel(data, u, v);
                        break;
                    case 7:
                        normal = geometry[i]->getNormal(X);
                        cone = *reinterpret_cast<Cone *>((geometry[i].get()));
                        cone.get_uv(normal, cone.get_vertex_Y_coord() - X.getCoord()[1], u, v);
                        color = get_pixel(data, u, v);
                        break;
                    case 8:
                    case 9:
                    case 10:
                        color.setR(93);
                        color.setG(173);
                        color.setB(164);
                        break;

                    case 11:
                        color.setR(255);
                        color.setG(143);
                        color.setB(0);
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