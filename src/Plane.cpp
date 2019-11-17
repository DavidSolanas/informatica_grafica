/****************************************+
 * Fichero: Plane.cpp
 * Autor: David Solanas
 *****************************************/

#include "Plane.hpp"
#include <cmath>
#include <iostream>

float max(float a, float b, float c)
{
    if (a > b && a > c)
        return a;
    else if (b > a && b > c)
        return b;
    return c;
}

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

BoundedPlane::BoundedPlane()
{
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

void BoundedPlane::get_uv(const Point &p, float &u, float &v)
{
    float w = (B - A).mod(),
          h = (D - A).mod();
    u = (p.getCoord()[0] - A.getCoord()[0]) / w;
    v = (p.getCoord()[1] - D.getCoord()[1]) / h;
}

Triangle::Triangle()
{
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

void Triangle::get_uv(const Point &p, float &u, float &v)
{
    float ba, bb, bc;
    std::array<float, 4> ca = A.getCoord(),
                         cb = B.getCoord(),
                         cc = C.getCoord();
    float xmax = max(ca[0], cb[0], cc[0]);
    float ymax = max(ca[1], cb[1], cc[1]);
    float abc_area = cross(B - A, C - A).mod();

    ba = cross(A - C, p - C).mod() / abc_area;
    bb = cross(B - A, p - A).mod() / abc_area;
    bc = 1 - ba - bb;
    u = ba * (ca[0] / xmax) + bb * (cb[0] / xmax) + bc * (cc[0] / xmax);
    v = ba * (ca[1] / ymax) + bb * (cb[1] / ymax) + bc * (cc[1] / ymax);
}