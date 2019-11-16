/****************************************+
 * Fichero: Cone.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef CONE_HPP
#define CONE_HPP

#include "Geometry.hpp"

class Cone : public Geometry
{
private:
    float theta;
    Point center;
    float h;
    float r;

public:
    Cone(const Point &p, float h, float r);
    Cone(const Point &p, const Point &v, float r);
    ~Cone();
    float getAngle();
    float getHeight();
    float getRadius();
    Point getCenter();
    Direction getNormal(Point X) override;
    bool intersect(const Point &p, const Direction &D, float &t) override;
};

#endif // !CONE_HPP