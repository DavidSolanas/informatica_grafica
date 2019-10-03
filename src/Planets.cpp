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
    bool correct = fabsf((city - center).mod() - radius) > 1e-6;
    if (correct)
    {
        this->center = center;
        this->axis = axis;
        this->city = city;
    }
    std::cout << "Planeta no coherente" << std::endl;
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

Station::Station(const float &inc, const float &az, const Planet &p)
{
    this->inclination = inc;
    this->azimuth = az;
    this->planet = p;
}