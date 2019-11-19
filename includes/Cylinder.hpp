
/****************************************+
 * Fichero: Cylinder.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Geometry.hpp"

class Cylinder : public Geometry
{
private:
    Point c1;
    Point c2;
    float r;
    float h;

public:
    Cylinder();
    Cylinder(Point c1, Point c2, float r, float h);
    ~Cylinder();
    float getRadius();
    Direction getNormal(Point X) override;
    float get_base_Y_coord();
    bool intersect(const Point &p, const Direction &D, float &t) override;
    bool isInCylinder(const Point &p, const Direction &D, const Point &center, float &t);
    void get_uv(const Direction &n, const float h, float &u, float &v);
};

#endif // !CYLINDER_HPP