
/****************************************+
 * Fichero: Cylinder.hpp
 * Autor: David Solanas Sanz    738630
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
    Cylinder(Disk b1, Disk b2, float r, float h, BRDF *mat);
    Cylinder(Disk b1, Disk b2, float r, float h, BRDF *mat, bool texture, std::string texture_file);
    ~Cylinder();
    float getRadius();
    Direction getNormal(Point X) override;
    float get_base_Y_coord();
    bool intersect(Ray &ray) override;
    bool isInCylinder(const Point &p, const Direction &D, const Point &center, float &t);
    float get_area() override;
    void get_uv(const Point &X, const Direction &n, float &u, float &v) override;
};

#endif // !CYLINDER_HPP