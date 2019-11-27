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
    this->a = n_normalized.x;
    this->b = n_normalized.y;
    this->c = n_normalized.z;
    this->d = -dot(n_normalized, o);
}

Plane::Plane(const Point &a, const Point &b, const Point &c)
{
    Direction n = normalize(cross(b - a, c - a));
    this->a = n.x;
    this->b = n.y;
    this->c = n.z;
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

float Plane::get_area()
{
    return INFINITY;
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

float BoundedPlane::get_area()
{
    return (A - B).mod() * (C - B).mod();
}

void BoundedPlane::get_uv(const Point &p, float &u, float &v)
{
    float w = (B - A).mod(),
          h = (D - A).mod();
    u = (p.x - A.x) / w;
    v = (p.y - D.y) / h;
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

float Triangle::get_area()
{
    float a = (B - A).mod();
    float b = (B - C).mod();
    float c = (C - A).mod();
    float s = (a + b + c) / 2; //semi-perimetro del triangulo
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

void Triangle::get_uv(const Point &p, float &u, float &v)
{
    float ba, bb, bc;
    float xmax = max(A.x, B.x, C.x);
    float ymax = max(A.y, B.y, C.y);
    float abc_area = cross(B - A, C - A).mod();

    ba = cross(A - C, p - C).mod() / abc_area;
    bb = cross(B - A, p - A).mod() / abc_area;
    bc = 1 - ba - bb;
    u = ba * (A.x / xmax) + bb * (B.x / xmax) + bc * (C.x / xmax);
    v = ba * (A.y / ymax) + bb * (B.y / ymax) + bc * (C.y / ymax);
}

Disk::Disk()
{
}

Disk::Disk(const Direction &n, const Point &p, const float r) : Plane(n, p)
{
    this->r = r;
    this->c = p;
}

Point Disk::getCenter()
{
    return this->c;
}

bool Disk::isInsideDisk(const Point &p)
{
    Direction d = p - this->c;
    return dot(d, d) <= r * r;
}

bool Disk::intersect(const Point &p, const Direction &D, float &t)
{
    t = -(dot(this->getNormal(), p) + this->d) / (dot(D, this->getNormal()));
    if (t <= 0)
        return false;
    return isInsideDisk(p + (D * t));
}

float Disk::get_area()
{
    return M_PI * r * r;
}

void Disk::get_uv(const Point &p, float &u, float &v)
{
    Direction n = this->getNormal(p);
    float x = p.x - c.x,
          y = p.y - c.y,
          z = p.z - c.z;
    float nx = abs(n.x),
          ny = abs(n.y),
          nz = abs(n.z);
    float cmax = max(nx, ny, nz);
    if (cmax == nx)
    {
        u = (y / r + 1) * 0.5;
        v = (z / r + 1) * 0.5;
    }
    if (cmax == ny)
    {
        u = (x / r + 1) * 0.5;
        v = (z / r + 1) * 0.5;
    }
    if (cmax == nz)
    {
        u = (x / r + 1) * 0.5;
        v = (y / r + 1) * 0.5;
    }
}