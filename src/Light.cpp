
/****************************************+
 * Fichero: Light.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Light.hpp"
#include <random>
#include <cmath>
#include <iostream>

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

Point PointLight::get_point_on_surface()
{
    return this->p;
}

int PointLight::get_number_of_samples()
{
    return 1;
}

float PointLight::get_incoming_light(const Point &X)
{
    return power / ((p - X).mod() * (p - X).mod());
}

bool PointLight::is_visible(const Point &X)
{
    return true;
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
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float u = dist(mt), v = dist(mt);
    return p.A + (p.B - p.A) * u + (p.D - p.A) * v;
}

int PlaneLight::get_number_of_samples()
{
    return p.get_area();
}

float PlaneLight::get_incoming_light(const Point &X)
{
    Point c = get_point_on_surface();
    return (power / get_number_of_samples()) / ((c - X).mod() * (c - X).mod());
}

bool PlaneLight::is_visible(const Point &X)
{
    return true;
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

int SphereLight::get_number_of_samples()
{
    return s.get_area();
}

float SphereLight::get_incoming_light(const Point &X)
{
    Point c = get_point_on_surface();
    return (power / get_number_of_samples()) / ((c - X).mod() * (c - X).mod());
}

bool SphereLight::is_visible(const Point &X)
{
    return true;
}