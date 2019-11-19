
/****************************************+
 * Fichero: Plane.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Geometry.hpp"

#ifndef PLANE_HPP
#define PLANE_HPP

class Plane : public Geometry
{
protected:
    /**
     * Plane surface defined by a*x + b*y + c*z + d = 0,
     * <a,b,c> nonzero normal vector
     */
    float a;
    float b;
    float c;
    float d;

public:
    Plane();
    Plane(const Direction &n, const Point &o);
    Plane(const Point &a, const Point &b, const Point &c);
    bool isInPlane(const Point &p);
    bool intersect(const Point &p, const Direction &D, float &t) override;
    float getD();
    Direction getNormal();
    Direction getNormal(Point X) override;
};

class BoundedPlane : public Plane
{
private:
    Point A;
    Point B;
    Point C;
    Point D;

public:
    BoundedPlane();
    BoundedPlane(const Point &_A, const Point &_B, const Point &_C, const Point &_D);
    bool isInsidePlane(const Point &p);
    bool intersect(const Point &p, const Direction &D, float &t) override;
    void get_uv(const Point &p, float &u, float &v);
};

class Triangle : public Plane
{
private:
    Point A;
    Point B;
    Point C;

public:
    Triangle();
    Triangle(const Point &_A, const Point &_B, const Point &_C);
    bool isInsideTriangle(const Point &p);
    bool intersect(const Point &p, const Direction &D, float &t) override;
    void get_uv(const Point &p, float &u, float &v);
};

class Disk : public Plane
{
private:
    float r;
    Point c;

public:
    Disk();
    Disk(const Direction &n, const Point &p, const float r);
    bool isInsideDisk(const Point &p);
    bool intersect(const Point &p, const Direction &D, float &t) override;
    void get_uv(const Point &p, float &u, float &v);
};

#endif // !PLANE_HPP