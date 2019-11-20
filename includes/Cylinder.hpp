
/****************************************+
 * Fichero: Cylinder.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Plane.hpp"

class Cylinder : public Geometry
{
private:
    Disk b1;
    Disk b2;
    float r;
    float h;

public:
    Cylinder();
    Cylinder(Disk b1, Disk b2, float r, float h);
    ~Cylinder();
    float getRadius();
    Direction getNormal(Point X) override;
    float get_base_Y_coord();
    bool intersect(const Point &p, const Direction &D, float &t) override;
    bool isInCylinder(const Point &p, const Direction &D, const Point &center, float &t);
    void get_uv(const Direction &n, const float h, float &u, float &v);
};

#endif // !CYLINDER_HPP