
/****************************************+
 * Fichero: Plane.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Geometry.hpp"

#ifndef PLANE_HPP
#define PLANE_HPP

class Plane : public Object
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
    Plane(const Direction &n, const Point &o, BRDF *mat);
    Plane(const Direction &n, const Point &o, BRDF *mat, bool texture);
    Plane(const Point &a, const Point &b, const Point &c, BRDF *mat);
    Plane(const Point &a, const Point &b, const Point &c, BRDF *mat, bool texture);
    bool isInPlane(const Point &p);
    bool intersect(Ray &ray) override;
    float get_area() override;
    float getD();
    Direction getNormal();
    Direction getNormal(Point X) override;
    void get_uv(const Point &X, const Direction &n, float &u, float &v) override;
};

class BoundedPlane : public Plane
{
public:
    /**
     * A               B
     * #################
     * #               #
     * #               #
     * #################
     * D               C
     */
    Point A;
    Point B;
    Point C;
    Point D;

    BoundedPlane();
    BoundedPlane(const Point &_A, const Point &_B, const Point &_C, const Point &_D, BRDF *mat);
    BoundedPlane(const Point &_A, const Point &_B, const Point &_C, const Point &_D, BRDF *mat, bool texture);
    bool isInsidePlane(const Point &p);
    bool intersect(Ray &ray) override;
    float get_area() override;
    void get_uv(const Point &X, const Direction &n, float &u, float &v) override;
};

class Triangle : public Plane
{
public:
    Point A;
    Point B;
    Point C;

    Triangle();
    Triangle(const Point &_A, const Point &_B, const Point &_C, BRDF *mat);
    Triangle(const Point &_A, const Point &_B, const Point &_C, BRDF *mat, bool texture);
    bool isInsideTriangle(const Point &p);
    bool intersect(Ray &ray) override;
    float get_area() override;
    void get_uv(const Point &X, const Direction &n, float &u, float &v) override;
};

class Disk : public Plane
{
public:
    float r;
    Point c;

    Disk();
    Disk(const Direction &n, const Point &p, const float r, BRDF *mat);
    Disk(const Direction &n, const Point &p, const float r, BRDF *mat, bool texture);
    bool isInsideDisk(const Point &p);
    Point getCenter();
    bool intersect(Ray &ray) override;
    float get_area() override;
    void get_uv(const Point &X, const Direction &n, float &u, float &v) override;
};

#endif // !PLANE_HPP