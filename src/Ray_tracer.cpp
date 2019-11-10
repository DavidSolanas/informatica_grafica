/****************************************+
 * Fichero: Ray_tracer.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Ray_tracer.hpp"
#include <cmath>
#include "Sphere.hpp"
#include "Plane.hpp"
#include <iostream>
#include <vector>

float Phong_BRDF(const float kd, const float ks, const float alpha, Direction n,
                 Direction wi, Direction wo)
{
    Direction wr = wi - (n * 2 * (dot(wi, n)));
    return (kd / M_PI) + (ks * (alpha + 2) * powf(abs(dot(wr, wo)), alpha) / (2 * M_PI));
}

float Lambertian_BRDF(const float kd)
{
    return (kd / M_PI);
}

void ray_tracer(std::string filename, Camera c, const int W, const int H)
{
    std::vector<Sphere> spheres;
    float split = W / 5;
    for (int i = 0; i < 5; i++)
    {
        Sphere s(Point((i + 1) * split - (split / 2), H / 2, c.getF().mod()), Direction(0, 200, 0),
                 Point((i + 1) * split - (split / 2) + 100, H / 2, c.getF().mod()));
        spheres.push_back(s);
    }
    //Plane p(Direction(-1, -1, -1), Point(200, 200, 20));

    Point light(0, 2 * H, c.getF().mod() - 100);
    float power = 10000000;

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
                Point pixel(x + 0.5, y + 0.5, c.getF().mod());
                Direction d_ray = normalize(pixel - c.getO());
                float t, tmin = INFINITY;
                int i = -1;
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
                    spheres[i].intersect(c.getO(), d_ray, t);
                    Point X = c.getO() + d_ray * t;
                    float Li = power / ((light - X).mod() * (light - X).mod());
                    Direction normal = normalize(X - spheres[i].getCenter());
                    Direction wi = normalize(light - X);
                    Direction wo = normalize(c.getO() - X);
                    float brdf = Phong_BRDF(0.8f, 0.5f, 5.0f, normal, wi, wo);
                    float geometry = abs(dot(normal, wi));
                    _f << Li * brdf * geometry * 50 << " "
                       << Li * brdf * geometry * 205 << " "
                       << Li * brdf * geometry * 50 << "\t";
                }
                else
                {
                    _f << "0 0 0\t";
                }
            }
            _f << "\n";
        }
    }
    _f.close();
}
