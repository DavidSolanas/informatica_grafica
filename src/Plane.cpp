/****************************************+
 * Fichero: Plane.cpp
 * Autor: David Solanas
 *****************************************/

#include "Plane.hpp"
#include <cmath>

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

Plane::Plane(const Point &a, const Point &b, const Point &c)
{
    Direction n = normalize(cross(b - a, c - a));
    std::array<float, 4> cn = n.getCoord();
    this->a = cn[0];
    this->b = cn[1];
    this->c = cn[2];
    this->d = -dot(n, a);
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
    return dot(this->getNormal(), p) + this->d == 0;
}

bool Plane::intersect(const Point &p, const Direction &D, float &t)
{
    t = -(dot(this->getNormal(), p) + this->d) / (dot(D, this->getNormal()));
    if (t <= 0)
        return false;
    return true;
}

BoundedPlane::BoundedPlane(const Point &_A, const Point &_B,
                           const Point &_C, const Point &_D) : Plane(_A, _B, _C)
{
    this->A = _A;
    this->B = _B;
    this->C = _C;
    this->D = _D;
}

/**
 * D = (x2 - x1) * (yp - y1) - (xp - x1) * (y2 - y1)
 */
bool BoundedPlane::isInsidePlane(const Point &p)
{
    float s1 = dot(cross(B - A, p - A), this->getNormal());
    float s2 = dot(cross(C - B, p - B), this->getNormal());
    float s3 = dot(cross(D - C, p - C), this->getNormal());
    float s4 = dot(cross(A - D, p - D), this->getNormal());

    if (s1 < 0 || s2 < 0 || s3 < 0 || s4 < 0)
    {
        return false;
    }
    return true;
}

bool BoundedPlane::intersect(const Point &p, const Direction &D, float &t)
{
    t = -(dot(this->getNormal(), p) + this->d) / (dot(D, this->getNormal()));
    if (t <= 0)
        return false;
    return isInsidePlane(p + (D * t));
}

Triangle::Triangle(const Point &_A, const Point &_B, const Point &_C) : Plane(_A, _B, _C)
{
    this->A = _A;
    this->B = _B;
    this->C = _C;
}

bool Triangle::isInsideTriangle(const Point &p)
{
    float s1 = dot(cross(B - A, p - A), this->getNormal());
    float s2 = dot(cross(C - B, p - B), this->getNormal());
    float s3 = dot(cross(A - C, p - C), this->getNormal());

    if (s1 < 0 || s2 < 0 || s3 < 0)
    {
        return false;
    }
    return true;
}

bool Triangle::intersect(const Point &p, const Direction &D, float &t)
{
    t = -(dot(this->getNormal(), p) + this->d) / (dot(D, this->getNormal()));
    if (t <= 0)
        return false;
    return isInsideTriangle(p + (D * t));
}
