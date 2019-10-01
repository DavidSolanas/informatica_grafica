/****************************************+
 * Fichero: Planets.cpp
 * Autor: David Solanas
 *****************************************/

#include "Planets.hpp"

Planet::Planet() {}

Planet::Planet(const Point &center, const Direction &axis, const Point &city)
{
    this->center = center;
    this->axis = axis;
    this->city = city;
}

Planet &Planet::operator=(const Planet &p)
{
    this->center = p.center;
    this->axis = p.axis;
    this->city = p.city;
    return *this;
}

Station::Station(const float &inc, const float &az, const Planet &p)
{
    this->inclination = inc;
    this->azimuth = az;
    this->planet = p;
}