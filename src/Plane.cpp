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

Plane::Plane(const Direction &n, const Point &o, BRDF *mat) : Object(mat)
{
    Direction n_normalized = normalize(n);
    this->a = n_normalized.x;
    this->b = n_normalized.y;
    this->c = n_normalized.z;
    this->d = -dot(n_normalized, o);
}

Plane::Plane(const Direction &n, const Point &o, BRDF *mat, bool texture, std::string texture_file) : Object(mat, texture, texture_file)
{
    Direction n_normalized = normalize(n);
    this->a = n_normalized.x;
    this->b = n_normalized.y;
    this->c = n_normalized.z;
    this->d = -dot(n_normalized, o);
}

Plane::Plane(const Point &a, const Point &b, const Point &c, BRDF *mat) : Object(mat)
{
    Direction n = normalize(cross(b - a, c - a));
    this->a = n.x;
    this->b = n.y;
    this->c = n.z;
    this->d = -dot(n, a);
}

Plane::Plane(const Point &a, const Point &b, const Point &c, BRDF *mat, bool texture, std::string texture_file) : Object(mat, texture, texture_file)
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
    return dot(this->getNormal(), p) + this->d < SMALLEST_DIST;
}

bool Plane::intersect(Ray &ray)
{
    float t = -(dot(getNormal(), ray.get_origin()) + this->d) / (dot(ray.get_direction(), getNormal()));
    if (t <= 0)
        return false;
    ray.set_parameter(t);
    return true;
}

void Plane::get_uv(const Point &X, const Direction &n, float &u, float &v)
{
}

float Plane::get_area()
{
    return INFINITY;
}

BoundedPlane::BoundedPlane()
{
}

BoundedPlane::BoundedPlane(const Point &_A, const Point &_B,
                           const Point &_C, const Point &_D, BRDF *mat) : Plane(_A, _B, _C, mat)
{
    this->A = _A;
    this->B = _B;
    this->C = _C;
    this->D = _D;
}

BoundedPlane::BoundedPlane(const Point &_A, const Point &_B,
                           const Point &_C, const Point &_D, BRDF *mat, bool texture, std::string texture_file) : Plane(_A, _B, _C, mat, texture, texture_file)
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

bool BoundedPlane::intersect(Ray &ray)
{
    bool intersect_plane = Plane::intersect(ray);
    if (intersect_plane)
    {
        return isInsidePlane(ray.get_position());
    }
    return false;
}

float BoundedPlane::get_area()
{
    return (A - B).mod() * (C - B).mod();
}

void BoundedPlane::get_uv(const Point &X, const Direction &n, float &u, float &v)
{
    float nx = fabs(n.x),
          ny = fabs(n.y),
          nz = fabs(n.z);
    float cmax = max(nx, ny, nz);
    float w = (B - A).mod(),
          h = (D - A).mod();
    if (cmax == nx)
    {
        u = fabs(X.z - A.z) / w;
        v = fabs(X.y - D.y) / h;
    }
    if (cmax == ny)
    {
        u = fabs(X.x - A.x) / w;
        v = fabs(X.z - D.z) / h;
    }
    if (cmax == nz)
    {
        u = fabs(X.x - A.x) / w;
        v = fabs(X.y - D.y) / h;
    }
}

Triangle::Triangle()
{
}

Triangle::Triangle(const Point &_A, const Point &_B, const Point &_C,
                   BRDF *mat) : Plane(_A, _B, _C, mat)
{
    this->A = _A;
    this->B = _B;
    this->C = _C;
}

Triangle::Triangle(const Point &_A, const Point &_B, const Point &_C,
                   BRDF *mat, bool texture, std::string texture_file) : Plane(_A, _B, _C, mat, texture, texture_file)
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

bool Triangle::intersect(Ray &ray)
{
    bool intersect_plane = Plane::intersect(ray);
    if (intersect_plane)
    {
        return isInsideTriangle(ray.get_position());
    }
    return false;
}

float Triangle::get_area()
{
    float a = (B - A).mod();
    float b = (B - C).mod();
    float c = (C - A).mod();
    float s = (a + b + c) / 2; //semi-perimetro del triangulo
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

void Triangle::get_uv(const Point &X, const Direction &n, float &u, float &v)
{
    float ba, bb, bc;
    float xmax = max(A.x, B.x, C.x);
    float ymax = max(A.y, B.y, C.y);
    float abc_area = cross(B - A, C - A).mod();

    ba = cross(A - C, X - C).mod() / abc_area;
    bb = cross(B - A, X - A).mod() / abc_area;
    bc = 1 - ba - bb;
    u = ba * (A.x / xmax) + bb * (B.x / xmax) + bc * (C.x / xmax);
    v = ba * (A.y / ymax) + bb * (B.y / ymax) + bc * (C.y / ymax);
}

Disk::Disk()
{
}

Disk::Disk(const Direction &n, const Point &p, const float r, BRDF *mat) : Plane(n, p, mat)
{
    this->r = r;
    this->c = p;
}

Disk::Disk(const Direction &n, const Point &p, const float r, BRDF *mat, bool texture, std::string texture_file) : Plane(n, p, mat, texture, texture_file)
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

bool Disk::intersect(Ray &ray)
{
    bool intersect_plane = Plane::intersect(ray);
    if (intersect_plane)
    {
        return isInsideDisk(ray.get_position());
    }
    return false;
}

float Disk::get_area()
{
    return M_PI * r * r;
}

void Disk::get_uv(const Point &X, const Direction &n, float &u, float &v)
{
    Direction _n = this->getNormal(X);
    float x = X.x - c.x,
          y = X.y - c.y,
          z = X.z - c.z;
    float nx = fabs(_n.x),
          ny = fabs(_n.y),
          nz = fabs(_n.z);
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