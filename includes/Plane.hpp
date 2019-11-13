
/****************************************+
 * Fichero: Plane.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Geometry.hpp"

#ifndef PLANE_HPP
#define PLANE_HPP

class Plane : public Geometry
{
private:
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
    bool isInPlane(const Point &p);
    bool intersect(const Point &p, const Direction &D, float &t) override;
    float getD();
    Direction getNormal();
    Direction getNormal(Point X) override;
};

#endif // !PLANE_HPP