/****************************************+
 * Fichero: Cone.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef CONE_HPP
#define CONE_HPP

#include "Geometry.hpp"

class Cone : public Geometry
{
public:
    float theta;
    Point vertex;
    float h;
    float r;

    Cone();
    Cone(const Point &p, float h, float r);
    ~Cone();
    float getAngle();
    float getHeight();
    float getRadius();
    Point getVertex();
    float get_vertex_Y_coord();
    Direction getNormal(Point X) override;
    bool intersect(Ray &ray) override;
    float get_area() override;
    void get_uv(const Direction &n, const float h, float &u, float &v);
};

#endif // !CONE_HPP