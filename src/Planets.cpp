/****************************************+
 * Fichero: Planets.cpp
 * Autor: David Solanas
 *****************************************/

#include "Planets.hpp"
#include <iostream>
#include <cmath>

Planet::Planet() {}

Planet::Planet(const Point &center, const Direction &axis, const Point &city)
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

Planet &Planet::operator=(const Planet &p)
{
    this->center = p.center;
    this->axis = p.axis;
    this->city = p.city;
    return *this;
}

Point Planet::getCenter()
{
    return this->center;
}

Direction Planet::getAxis()
{
    return this->axis;
}

Point Planet::getCity()
{
    return this->city;
}

float Planet::getRadius()
{
    return this->axis.mod() / 2;
}

Station::Station(const float &inc, const float &az, const Planet &p)
{
    this->inclination = inc;
    this->azimuth = az;
    this->planet = p;
}

/**
 * Devuelve la posición en el planeta de la estación
 */
Point Station::getPosition()
{
    return getSurfacePoint(this->planet, this->azimuth, this->inclination);
}

/**
 * Devuelve la normal a la superficie de la estación
 * (módulo 1), coordenada K
 */
Direction Station::getNormal()
{
    float x = cos(this->azimuth) * sin(this->inclination);
    float y = sin(this->azimuth) * sin(this->inclination);
    float z = cos(this->inclination);
    return Direction(x, y, z);
}

/**
 * Devuelve el vector tangente a la superficie del
 * planeta y perpendicular al eje del planeta (mod 1).
 * Coordenada I
 */
Direction Station::getLongitudeTD()
{
    float x = -sin(this->azimuth);
    float y = cos(this->azimuth);
    return Direction(x, y, 0);
}

/**
 * Devuelve el vector tangente a la superficie del
 * planeta y perpendicular al otro vector tangente (Longitude),
 * (mod 1). Coordenada J
 */
Direction Station::getLatitudeTD()
{
    float x = cos(this->azimuth) * cos(this->inclination);
    float y = sin(this->azimuth) * cos(this->inclination);
    float z = -sin(this->inclination);
    return Direction(x, y, z);
}

/**
 * Devuelve un punto en la superficie del planeta
 * según su azimuth e inclination
 */
Point getSurfacePoint(Planet p, const float az, const float inc)
{
    float r = p.getRadius();
    float x = r * cos(az) * sin(inc);
    float y = r * sin(az) * sin(inc);
    float z = r * cos(inc);
    return Point(x, y, z);
}