
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
    virtual Point get_point_on_surface() = 0;
    virtual int get_number_of_samples() = 0;
    virtual float get_incoming_light(const Point &X) = 0;
    virtual bool is_visible(const Point &X) = 0;
    ~Light();
};

class PointLight : public Light
{
public:
    Point p;
    PointLight(const Point &p, const float power, const RGB &color);
    Point get_point_on_surface() override;
    int get_number_of_samples() override;
    float get_incoming_light(const Point &X) override;
    bool is_visible(const Point &X) override;
    ~PointLight();
};

class PlaneLight : public Light
{
public:
    BoundedPlane p;
    PlaneLight(const BoundedPlane &p, const float power, const RGB &color);
    Point get_point_on_surface() override;
    float get_incoming_light(const Point &X) override;
    int get_number_of_samples() override;
    bool is_visible(const Point &X) override;
    ~PlaneLight();
};

class SphereLight : public Light
{
public:
    Sphere s;
    SphereLight(const Sphere &s, const float power, const RGB &color);
    Point get_point_on_surface() override;
    float get_incoming_light(const Point &X) override;
    int get_number_of_samples() override;
    bool is_visible(const Point &X) override;
    ~SphereLight();
};

#endif //!LIGHT_HPP