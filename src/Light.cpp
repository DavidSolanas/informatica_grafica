
/****************************************+
 * Fichero: Light.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Light.hpp"
#include <random>
#include <cmath>
#include <iostream>

const int N_SAMPLES = 5;

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
    RGB Li(0, 0, 0);
    RGB tot_Li(0, 0, 0);
    for (int i = 0; i < N_SAMPLES; i++)
    {
        Point c = get_point_on_surface();
        Li = (get_light_amount() / N_SAMPLES) / ((c - X).mod() * (c - X).mod());
        Direction wi = normalize(c - X);
        float g1 = std::max(0.0f, dot(normal, wi));
        Direction wi_s = normalize(X - c);
        float g2 = std::max(0.0f, dot(p.getNormal(), wi_s));
        tot_Li = tot_Li + Li * g1 * g2;
    }
    return tot_Li;
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
    RGB Li(0, 0, 0), tot_Li(0, 0, 0);
    for (int i = 0; i < N_SAMPLES; i++)
    {
        Point c = get_point_on_surface();
        Li = (get_light_amount() / N_SAMPLES) / ((c - X).mod() * (c - X).mod());
        Direction wi = normalize(c - X);
        float g1 = std::max(0.0f, dot(normal, wi));
        Direction wi_s = normalize(X - c);
        float g2 = std::max(0.0f, dot(s.getNormal(c), wi_s));
        tot_Li = tot_Li + Li * g1 * g2;
    }
    return tot_Li;
}

bool SphereLight::is_visible(const Point &X)
{
    return true;
}

Object *SphereLight::get_object()
{
    return &s;
}