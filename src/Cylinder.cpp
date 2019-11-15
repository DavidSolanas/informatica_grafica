
/****************************************+
 * Fichero: Cylinder.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Cylinder.hpp"
#include <cmath>
#include <iostream>

Cylinder::Cylinder(Point c1, Point c2, float r, float h)
{
    this->c1 = c1;
    this->c2 = c2;
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

Direction Cylinder::getNormal(Point X)
{
    //Comprobar si el punto está en una de las bases
    float px = X.getCoord()[0], py = X.getCoord()[1], pz = X.getCoord()[2];
    if (abs(px - c1.getCoord()[0]) <= r &&
        abs(pz - c1.getCoord()[2]) <= r)
    {
        if (py == c1.getCoord()[1] + h)
        {
            return Direction(0, 1, 0);
        }
        if (py == c1.getCoord()[1])
        {
            return Direction(0, -1, 0);
        }
    }

    //El punto está en el lateral
    // C será el punto central a la misma altura que el punto X
    Point C(c1.getCoord()[0], py, c1.getCoord()[2]);
    return normalize(X - C);
}

bool Cylinder::intersect(const Point &p, const Direction &D, float &t)
{
    // Trasladamos el rayo para que el centro de la base del cilindro
    // esté en el origen del rayo punto p)
    Point p0(p.getCoord()[0] - c1.getCoord()[0],
             p.getCoord()[1] - c1.getCoord()[1],
             p.getCoord()[2] - c1.getCoord()[2]);

    // Comprobar si intersecta con el cilindro infinito
    float a = D.getCoord()[0] * D.getCoord()[0] + D.getCoord()[2] * D.getCoord()[2];
    float b = D.getCoord()[0] * p0.getCoord()[0] + D.getCoord()[2] * p0.getCoord()[2];
    float c = p0.getCoord()[0] * p0.getCoord()[0] + p0.getCoord()[2] * p0.getCoord()[2] - this->r * this->r;

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
    float y = p0.getCoord()[1] + t * D.getCoord()[1];

    //Comprobar si intersecta con alguna de las bases
    if (y > h || y < 0)
    {
        float t1;
        bool base1 = isInCylinder(p, D, c2, t1);
        if (base1)
            t = t1;
        bool base2 = isInCylinder(p, D, c1, t1);
        if (base2 && t1 > 0 && t >= t1)
            t = t1;
        return base1 || base2;
    }

    return true;
}

bool Cylinder::isInCylinder(const Point &p, const Direction &D, const Point &center, float &t)
{
    Direction n = getNormal(center);
    // Trasladamos el rayo para que el centro de la base del cilindro
    // esté en el origen del rayo punto p)
    Point p0(p.getCoord()[0] - c1.getCoord()[0],
             p.getCoord()[1] - c1.getCoord()[1],
             p.getCoord()[2] - c1.getCoord()[2]);

    Direction v = center - c1;
    float d = -dot(n, v);

    t = -(dot(n, p0) + d) / (dot(D, n));
    if (t <= 0)
        return false;

    //Comprobar si el punto de intersección está dentro del círculo
    Point p1 = p0 + D * t;
    if (p1.getCoord()[0] * p1.getCoord()[0] + p1.getCoord()[2] * p1.getCoord()[2] - r * r > 0)
        return false;

    return true;
}