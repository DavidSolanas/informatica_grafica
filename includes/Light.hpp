
/****************************************+
 * Fichero: Light.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "RGB.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

class Light
{
public:
    float power;
    RGB color;
    Light(const float p, const RGB &c);
    ~Light();
};

class PointLight : public Light
{
public:
    Point p;
    PointLight(const Point &p, const float power, const RGB &color);
    ~PointLight();
};

class PlaneLight : public Light
{
public:
    BoundedPlane p;
    PlaneLight(const BoundedPlane &p, const float power, const RGB &color);
    Point get_point_on_surface();
    ~PlaneLight();
};

class SphereLight : public Light
{
public:
    Sphere s;
    SphereLight(const Sphere &s, const float power, const RGB &color);
    Point get_point_on_surface();
    ~SphereLight();
};

#endif //!LIGHT_HPP