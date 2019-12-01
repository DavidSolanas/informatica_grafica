
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

float PointLight::get_incoming_light(const Point &X, const Direction &normal)
{
    Point c = get_point_on_surface();
    float Li = power / ((p - X).mod() * (p - X).mod());
    Direction wi = normalize(c - X);
    float g = dot(normal, wi) < 0 ? 0 : dot(normal, wi);
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

float PlaneLight::get_incoming_light(const Point &X, const Direction &normal)
{
    float Li = 0.0f;
    float tot_Li = 0.0f;
    for (int i = 0; i < 6; i++)
    {
        Point c = get_point_on_surface();
        Li = (power / 6) / ((c - X).mod() * (c - X).mod());
        Direction wi = normalize(c - X);
        float g1 = dot(normal, wi) < 0 ? 0 : dot(normal, wi);
        Direction wi_s = normalize(X - c);
        float g2 = dot(p.getNormal(), wi_s) < 0 ? 0 : dot(p.getNormal(), wi_s);
        tot_Li += Li * g1 * g2;
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

float SphereLight::get_incoming_light(const Point &X, const Direction &normal)
{
    float Li = 0.0f, tot_Li = 0.0f;
    for (int i = 0; i < get_number_of_samples(); i++)
    {
        Point c = get_point_on_surface();
        Li = (power / get_number_of_samples()) / ((c - X).mod() * (c - X).mod());
        Direction wi = normalize(c - X);
        float g1 = dot(normal, wi) < 0 ? 0 : dot(normal, wi);
        Direction wi_s = normalize(X - c);
        float g2 = dot(s.getNormal(c), wi_s) < 0 ? 0 : dot(s.getNormal(c), wi_s);
        tot_Li += Li * g1 * g2;
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