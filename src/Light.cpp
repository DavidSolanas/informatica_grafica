
/****************************************+
 * Fichero: Light.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Light.hpp"
#include <random>
#include <cmath>

Light::Light(const float p, const RGB &c)
{
    this->power = p;
    this->color = c;
}

Light::~Light()
{
}

PointLight::PointLight(const Point &p, const float power, const RGB &color) : Light(power, color)
{
    this->p = p;
}

PointLight::~PointLight()
{
}

PlaneLight::PlaneLight(const BoundedPlane &p, const float power, const RGB &color) : Light(power, color)
{
    this->p = p;
}

PlaneLight::~PlaneLight()
{
}

Point PlaneLight::get_point_on_surface()
{
    Direction d = normalize(get_random_vect());
    while (dot(p.getNormal(), d) == p.getNormal().mod() * d.mod())
    {
        d = normalize(get_random_vect());
    }
    Direction v = normalize(cross(p.getNormal(), d));
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist;
    float x = dist(mt);
    Point p(v.getCoord()[0] * x, v.getCoord()[1] * x, v.getCoord()[2] * x);
    while (!this->p.isInsidePlane(p))
    {
        x = dist(mt);
        p = Point(v.getCoord()[0] * x, v.getCoord()[1] * x, v.getCoord()[2] * x);
    }
    return p;
}

SphereLight::SphereLight(const Sphere &s, const float power, const RGB &color) : Light(power, color)
{
    this->s = s;
}

SphereLight::~SphereLight()
{
}

Point SphereLight::get_point_on_surface()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float u = dist(mt), v = dist(mt);
    float th = 2 * M_PI * u;
    float phi = acos(2 * v - 1);
    return getSurfacePoint(s, th, phi);
}