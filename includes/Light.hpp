
/****************************************+
 * Fichero: Light.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "RGB.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

class World;
class Light
{
public:
    float power;
    RGB color;
    Light(const float p, const RGB &c);
    RGB get_light_amount() const;
    virtual Point get_point_on_surface() = 0;
    virtual int get_number_of_samples() = 0;
    virtual RGB get_incoming_light(const World &w, const Point &X, const Direction &normal) = 0;
    virtual bool is_visible(const Point &X, const World &w) = 0;
    virtual Object *get_object() = 0;
    ~Light();
};

class PointLight : public Light
{
public:
    Point p;
    PointLight(const Point &p, const float power, const RGB &color);
    Point get_point_on_surface() override;
    int get_number_of_samples() override;
    RGB get_incoming_light(const World &w, const Point &X, const Direction &normal) override;
    bool is_visible(const Point &X, const World &w) override;
    Object *get_object() override;
    ~PointLight();
};

class PlaneLight : public Light
{
public:
    BoundedPlane p;
    PlaneLight(const BoundedPlane &p, const float power, const RGB &color);
    Point get_point_on_surface() override;
    RGB get_incoming_light(const World &w, const Point &X, const Direction &normal) override;
    int get_number_of_samples() override;
    bool is_visible(const Point &X, const World &w) override;
    Object *get_object() override;
    ~PlaneLight();
};

class SphereLight : public Light
{
public:
    Sphere s;
    SphereLight(const Sphere &s, const float power, const RGB &color);
    Point get_point_on_surface() override;
    RGB get_incoming_light(const World &w, const Point &X, const Direction &normal) override;
    int get_number_of_samples() override;
    bool is_visible(const Point &X, const World &w) override;
    Object *get_object() override;
    ~SphereLight();
};

#endif //!LIGHT_HPP