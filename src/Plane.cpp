/****************************************+
 * Fichero: Plane.cpp
 * Autor: David Solanas
 *****************************************/

#include "Plane.hpp"

Plane::Plane() {}

Plane::Plane(const Direction &n, const Point &o)
{
    Direction n_normalized = normalize(n);
    std::array<float, 4> cn = n_normalized.getCoord();
    this->a = cn[0];
    this->b = cn[1];
    this->c = cn[2];
    this->d = -dot(n_normalized, o);
}

float Plane::getD()
{
    return this->d;
}

Direction Plane::getNormal()
{
    return Direction(this->a, this->b, this->c);
}

Direction Plane::getNormal(Point X)
{
    return this->getNormal();
}

bool Plane::isInPlane(const Point &p)
{
    return dot(Direction(this->a, this->b, this->c), p) + this->d == 0;
}

bool Plane::intersect(const Point &p, const Direction &D, float &t)
{
    t = -(dot(this->getNormal(), p) + this->d) / (dot(D, this->getNormal()));
    if (t == 0)
        return false;
    return true;
}