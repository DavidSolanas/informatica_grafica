/****************************************+
 * Fichero: Spheres.cpp
 * Autor: David Solanas
 *****************************************/

#include "Sphere.hpp"
#include <iostream>
#include <cmath>
#include "Transformation.hpp"

Sphere::Sphere() {}

Sphere::Sphere(const Point &center, const Direction &axis, const Point &city,
               const RGB &c, const float ior) : Object(c)
{
    idx_of_refraction = ior;
    float radius = axis.mod() / 2;
    bool correct = abs((city - center).mod() - radius) <= 1e-6;
    if (correct)
    {
        this->center = center;
        this->axis = axis;
        this->city = city;
    }
    else
    {
        std::cout << "Planeta no coherente" << std::endl;
    }
}

Sphere &Sphere::operator=(const Sphere &p)
{
    this->center = p.center;
    this->axis = p.axis;
    this->city = p.city;
    return *this;
}

Point Sphere::getCenter()
{
    return this->center;
}

Direction Sphere::getAxis()
{
    return this->axis;
}

Point Sphere::getCity()
{
    return this->city;
}

float Sphere::getRadius()
{
    return this->axis.mod() / 2;
}

const float Sphere::getAzimuth() const
{
    float x = city.x;
    float z = city.z;
    return atanf(x / -z);
}

void Sphere::get_uv(const Direction &n, float &u, float &v)
{
    u = atan2(n.x, n.z) / (2 * M_PI) + 0.5;
    v = 0.5 - asin(n.y) / M_PI;
}

SphereGeometry::SphereGeometry(const float &inc, const float &az, const Sphere &p)
{
    this->inclination = inc;
    this->azimuth = p.getAzimuth() - az;
    this->sphere = p;
}

/**
 * Devuelve la posición en el Spherea de la estación
 */
Point SphereGeometry::getPosition()
{
    return getSurfacePoint(this->sphere, this->azimuth, this->inclination);
}

/**
 * Devuelve la normal a la superficie de la estación
 * (módulo 1), coordenada K
 */
Direction SphereGeometry::getNormal()
{
    float x = sin(this->azimuth) * sin(this->inclination);
    float y = cos(this->inclination);
    float z = -cos(this->azimuth) * sin(this->inclination);
    return Direction(x, y, z);
}

/**
 * Devuelve el vector tangente a la superficie del
 * Spherea y perpendicular al eje del Spherea (mod 1).
 * Coordenada I
 */
Direction SphereGeometry::getLongitudeTD()
{
    float x = cos(this->azimuth);
    float z = sin(this->azimuth);
    return Direction(x, 0, z);
}

/**
 * Devuelve el vector tangente a la superficie del
 * Spherea y perpendicular al otro vector tangente (Longitude),
 * (mod 1). Coordenada J
 */
Direction SphereGeometry::getLatitudeTD()
{
    float x = sin(this->azimuth) * cos(this->inclination);
    float y = sin(this->inclination);
    float z = -cos(this->azimuth) * cos(this->inclination);
    return Direction(x, y, z);
}

/**
 * Devuelve un punto en la superficie del Spherea
 * según su azimuth e inclination
 */
Point getSurfacePoint(Sphere p, const float az, const float inc)
{
    //radius
    float r = p.getRadius();
    float x = p.center.x + r * sin(az) * sin(inc);
    float y = p.center.y + r * cos(inc);
    float z = p.center.z + r * -cos(az) * sin(inc);
    return Point(x, y, z);
}

Direction Sphere::getNormal(Point X)
{
    return normalize(X - this->center);
}

/**
 * R(t) = o + td
	(p-c)·(p-c) - r² = 0 =>
	(o + td - c) · (o + td - c) - r² = 0 =>
	t²(D·D) + 2t(d · (o - c)) + (o - c) · (o - c) - r² = 0
 */
bool Sphere::intersect(Ray &ray)
{
    Direction L = ray.get_origin() - center;
    float a = dot(ray.get_direction(), ray.get_direction());
    float b = 2 * dot(ray.get_direction(), L);
    float c = dot(L, L) - (this->getRadius() * this->getRadius());
    float d = (b * b) - (4 * a * c);
    if (d < 0)
        return false;

    float x1 = (-b + sqrt(d)) / (2 * a);
    float x2 = (-b - sqrt(d)) / (2 * a);
    float t = 0;
    if (x1 > x2)
        t = x2;
    if (t < 0)
        t = x1;
    if (t < 0)
        return false;
    ray.set_parameter(t);
    return true;
}

float Sphere::get_area()
{
    return 4 * M_PI * getRadius() * getRadius();
}