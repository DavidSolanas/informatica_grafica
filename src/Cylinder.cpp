
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

Cylinder::Cylinder(Disk b1, Disk b2, float r, float h)
{
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

bool Cylinder::intersect(const Point &p, const Direction &D, float &t)
{
    // Trasladamos el rayo para que el centro de la base del cilindro
    // esté en el origen del rayo punto p)
    Point p0(p.x - b1.getCenter().x,
             p.y - b1.getCenter().y,
             p.z - b1.getCenter().z);

    // Comprobar si intersecta con el cilindro infinito
    float a = D.x * D.x + D.z * D.z;
    float b = D.x * p0.x + D.z * p0.z;
    float c = p0.x * p0.x + p0.z * p0.z - this->r * this->r;

    float d = b * b - a * c;

    //No existen soluciones si el discriminante es negativo
    if (d < 0)
        return false;

    float x1 = (-b + sqrt(d)) / a;
    float x2 = (-b - sqrt(d)) / a;

    if (x1 > x2)
        t = x2;
    if (t < 0)
        t = x1;
    if (t < 0)
        return false;

    // Comprobar si la intersección está en el rango del cilindro
    float y = p0.y + t * D.y;

    //Comprobar si intersecta con alguna de las bases
    if (y > h || y < 0)
    {
        float t1;
        bool base1 = b2.intersect(p, D, t1);
        if (base1)
            t = t1;
        bool base2 = b1.intersect(p, D, t1);
        if (base2 && t1 > 0 && t >= t1)
            t = t1;
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