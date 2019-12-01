
/****************************************+
 * Fichero: Cylinder.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Cylinder.hpp"
#include <cmath>
#include <iostream>

Cylinder::Cylinder()
{
}

Cylinder::Cylinder(Disk b1, Disk b2, float r, float h, const RGB &c, const float ior) : Object(c)
{
    idx_of_refraction = ior;
    this->b1 = b1;
    this->b2 = b2;
    this->h = h;
    this->r = r;
}

Cylinder::~Cylinder()
{
}

float Cylinder::getRadius()
{
    return this->r;
}

float Cylinder::get_base_Y_coord()
{
    return b1.getCenter().y;
}

Direction Cylinder::getNormal(Point X)
{
    //Comprobar si el punto está en una de las bases
    float py = X.y;
    if (b1.isInsideDisk(X))
    {
        return b1.getNormal();
    }
    if (b2.isInsideDisk(X))
    {
        return b2.getNormal();
    }

    //El punto está en el lateral
    // C será el punto central a la misma altura que el punto X
    Direction d = b2.getCenter() - b1.getCenter();
    float x = d.x == 0 ? b1.getCenter().x : d.x,
          y = py,
          z = d.z == 0 ? b1.getCenter().z : d.z;
    Point C(x, y, z);
    return normalize(X - C);
}

bool Cylinder::intersect(Ray &ray)
{
    // Trasladamos el rayo para que el centro de la base del cilindro
    // esté en el origen del rayo punto p)
    Point p0(ray.get_origin().x - b1.getCenter().x,
             ray.get_origin().y - b1.getCenter().y,
             ray.get_origin().z - b1.getCenter().z);

    // Comprobar si intersecta con el cilindro infinito
    float a = ray.get_direction().x * ray.get_direction().x + ray.get_direction().z * ray.get_direction().z;
    float b = ray.get_direction().x * p0.x + ray.get_direction().z * p0.z;
    float c = p0.x * p0.x + p0.z * p0.z - this->r * this->r;

    float d = b * b - a * c;

    //No existen soluciones si el discriminante es negativo
    if (d < 0)
        return false;

    float x1 = (-b + sqrt(d)) / a;
    float x2 = (-b - sqrt(d)) / a;
    float t = 0;
    if (x1 > x2)
        t = x2;
    if (t < 0)
        t = x1;
    if (t < 0)
        return false;
    ray.set_parameter(t);
    // Comprobar si la intersección está en el rango del cilindro
    float y = p0.y + t * ray.get_direction().y;

    //Comprobar si intersecta con alguna de las bases
    if (y > h || y < 0)
    {
        Ray ray1 = ray;
        bool base1 = b2.intersect(ray1);
        if (base1)
            ray = ray1;
        bool base2 = b1.intersect(ray1);
        if (base2 && ray1.get_parameter() > 0 &&
            ray.get_parameter() >= ray1.get_parameter())
            ray = ray1;
        return base1 || base2;
    }
    return true;
}

float Cylinder::get_area()
{
    return (2 * M_PI * r * r) + (2 * M_PI * r * h);
}

void Cylinder::get_uv(const Direction &n, const float h, float &u, float &v)
{
    u = atan2(n.x, n.z) / (2 * M_PI) + 0.5;
    v = h / this->h;
}