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

    std::vector<Geometry *> geometry = scene3(c, W, H);

    float power = 3600000;
    PointLight light(Point(W / 2, H - 200, c.f.mod() - 500), power, RGB(255, 255, 255));

    std::ofstream _f(filename);
    if (_f.is_open())
    {
        _f << "P3" << std::endl;
        _f << W << " " << H << std::endl;
        _f << "255" << std::endl;
        int inicioX = c.o.x - c.l.mod();
        int finX = c.o.x + c.l.mod();
        int inicioY = c.o.y - c.u.mod();
        int finY = c.o.y + c.u.mod();
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
                    Point pixel(x + xrand, y + yrand, c.f.mod());
                    Direction d_ray = normalize(pixel - c.o);
                    Ray ray(c.o, d_ray);
                    float tmin = INFINITY;
                    for (int j = 0; j < geometry.size(); j++)
                    {
                        if (geometry[j]->intersect(ray))
                        {
                            if (ray.get_parameter() > 0 && ray.get_parameter() < tmin)
                            {
                                tmin = ray.get_parameter();
                                i = j;
                            }
                        }
                    }
                    if (i != -1)
                    {
                        ray.set_parameter(tmin);
                        // Calcular luz puntual
                        X = ray.get_position();
                        float Li = power / ((light.p - X).mod() * (light.p - X).mod());
                        Direction normal = geometry[i]->getNormal(X);
                        Direction wi = normalize(light.p - X);
                        Direction wo = normalize(c.o - X);
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
                        color.r = 240;
                        color.g = 240;
                        color.b = 240;
                        break;
                    case 1:
                        color.r = 240;
                        color.g = 240;
                        color.b = 240;
                        break;
                    case 2:
                        bp = *reinterpret_cast<BoundedPlane *>(geometry[i]);
                        bp.get_uv(X, u, v);
                        color = get_pixel(data, u, v);
                        break;
                    case 3:
                        color.r = 240;
                        color.g = 240;
                        color.b = 240;
                        break;
                    case 4:
                        color.r = 240;
                        color.g = 240;
                        color.b = 240;
                        break;
                    case 5:
                        normal = geometry[i]->getNormal(X);
                        cy = *reinterpret_cast<Cylinder *>(geometry[i]);
                        cy.get_uv(normal, X.y - cy.get_base_Y_coord(), u, v);
                        color = get_pixel(data, u, v);
                        break;
                    case 6:
                        s = *reinterpret_cast<Sphere *>(geometry[i]);
                        normal = normalize(s.getCenter() - X);
                        s.get_uv(normal, u, v);
                        color = get_pixel(data, u, v);
                        break;
                    case 7:
                        normal = geometry[i]->getNormal(X);
                        cone = *reinterpret_cast<Cone *>(geometry[i]);
                        cone.get_uv(normal, cone.get_vertex_Y_coord() - X.y, u, v);
                        color = get_pixel(data, u, v);
                        break;
                    case 8:
                    case 9:
                    case 10:
                        color.r = 93;
                        color.g = 173;
                        color.b = 164;
                        break;

                    case 11:
                        color.r = 255;
                        color.g = 143;
                        color.b = 0;
                        break;
                    case 14:
                        color.r = 0;
                        color.g = 255;
                        color.b = 0;
                        break;

                    default:
                        color.r = 141;
                        color.g = 141;
                        color.b = 141;
                        break;
                    }
                    Lo /= n_ray;
                    _f << Lo * color.r << " "
                       << Lo * color.g << " "
                       << Lo * color.b << "\t";
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