
/****************************************+
 * Fichero: Cylinder.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Plane.hpp"

class Cylinder : public Object
{
public:
    Disk b1;
    Disk b2;
    float r;
    float h;

    Cylinder();
    Cylinder(Disk b1, Disk b2, float r, float h, const RGB &c, const float ior = AIR_REFRACTION_INDEX);
    ~Cylinder();
    float getRadius();
    Direction getNormal(Point X) override;
    float get_base_Y_coord();
    bool intersect(Ray &ray) override;
    bool isInCylinder(const Point &p, const Direction &D, const Point &center, float &t);
    float get_area() override;
    void get_uv(const Direction &n, const float h, float &u, float &v);
};

#endif // !CYLINDER_HPP