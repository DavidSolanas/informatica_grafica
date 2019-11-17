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
    Point vertex;
    float h;
    float r;

public:
    Cone();
    Cone(const Point &p, float h, float r);
    ~Cone();
    float getAngle();
    float getHeight();
    float getRadius();
    Point getVertex();
    float get_vertex_Y_coord();
    Direction getNormal(Point X) override;
    bool intersect(const Point &p, const Direction &D, float &t) override;
    void get_uv(const Direction &n, const float h, float &u, float &v);
};

#endif // !CONE_HPP