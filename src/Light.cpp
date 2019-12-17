
/****************************************+
 * Fichero: Light.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/
#define _USE_MATH_DEFINES
#include "Light.hpp"
#include <random>
#include <cmath>
#include <iostream>

Light::Light(const float power, const RGB &c)
{
    this->power = power;
    this->color = c;
}

Light::~Light()
{
}

RGB Light::get_light_amount() const
{
    return color * power;
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

RGB PointLight::get_incoming_light(const Point &X, const Direction &normal)
{
    RGB Li = get_light_amount() / ((p - X).mod() * (p - X).mod());
    Direction wi = normalize(p - X);
    float g = std::max(0.0f, dot(normal, wi));
    return Li * g;
}

bool PointLight::is_visible(const Point &X)
{
    return true;
}

Object *PointLight::get_object()
{
    return nullptr;
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

RGB PlaneLight::get_incoming_light(const Point &X, const Direction &normal)
{
    return RGB(.0f, .0f, .0f);
}

bool PlaneLight::is_visible(const Point &X)
{
    return true;
}

Object *PlaneLight::get_object()
{
    return &p;
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

RGB SphereLight::get_incoming_light(const Point &X, const Direction &normal)
{
    return RGB(.0f, .0f, .0f);
}

bool SphereLight::is_visible(const Point &X)
{
    return true;
}

Object *SphereLight::get_object()
{
    return &s;
}