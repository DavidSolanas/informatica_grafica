/****************************************+
 * Fichero: Spheres.cpp
 * Autor: David Solanas
 *****************************************/

#include "Sphere.hpp"
#include <iostream>
#include <cmath>
#include "Transformation.hpp"

Sphere::Sphere() {}

Sphere::Sphere(const Point &center, const Direction &axis, const Point &city)
{
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
    std::array<float, 4> c = city.getCoord();
    float x = c[0];
    float z = c[2];
    return atanf(x / -z);
}

void Sphere::get_uv(const Direction &n, float &u, float &v)
{
    u = atan2(n.getCoord()[0], n.getCoord()[2]) / (2 * M_PI) + 0.5;
    v = 0.5 - asin(n.getCoord()[1]) / M_PI;
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
    //origin
    std::array<float, 4> o = p.getCenter().getCoord();
    //radius
    float r = p.getRadius();
    float x = o[0] + r * sin(az) * sin(inc);
    float y = o[1] + r * cos(inc);
    float z = o[2] + r * -cos(az) * sin(inc);
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
bool Sphere::intersect(const Point &p, const Direction &D, float &t)
{
    Direction L = p - center;
    float a = dot(D, D);
    float b = 2 * dot(D, L);
    float c = dot(L, L) - (this->getRadius() * this->getRadius());
    float d = (b * b) - (4 * a * c);
    if (d < 0)
        return false;

    float x1 = (-b + sqrt(d)) / (2 * a);
    float x2 = (-b - sqrt(d)) / (2 * a);
    if (x1 > x2)
        t = x2;
    if (t < 0)
        t = x1;
    if (t < 0)
        return false;
    return true;
}