/****************************************+
 * Fichero: Cone.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Cone.hpp"
#include <cmath>
#include <iostream>

Cone::Cone()
{
}

Cone::Cone(const Point &p, float h, float r)
{
    this->vertex = p;
    this->h = h;
    this->r = r;
    //Calcular el angulo de la parte superior del cono,
    //definir el punto superior y un punto en un extremo de
    //la base del cono y calcular el arccos(altura/lado)
    Point p1(p.getCoord()[0] + r, p.getCoord()[1], p.getCoord()[2]);
    Point p2(p.getCoord()[0], p.getCoord()[1] + h, p.getCoord()[2]);
    float lado = (p2 - p1).mod();
    this->theta = acos(h / lado);
}

Cone::~Cone()
{
}

float Cone::getAngle()
{
    return theta;
}

float Cone::getHeight()
{
    return h;
}

float Cone::getRadius()
{
    return r;
}

Point Cone::getVertex()
{
    return vertex;
}

float Cone::get_vertex_Y_coord()
{
    return vertex.getCoord()[1];
}

Direction Cone::getNormal(Point X)
{
    //Punto central a altura Xy
    Point p1(vertex.getCoord()[0], X.getCoord()[1], vertex.getCoord()[2]);
    //Punto superior
    Point p2(vertex.getCoord()[0], vertex.getCoord()[1] + h, vertex.getCoord()[2]);

    Direction w = X - vertex;
    std::array<float, 4> wcord = w.getCoord();
    wcord[1] = 0.0f;
    w.setCoord(wcord);
    w = normalize(w);

    return Direction(w.getCoord()[0] * h / r, r / h, w.getCoord()[2] * h / r);
}

bool Cone::intersect(const Point &p, const Direction &D, float &t)
{
    // Trasladamos el rayo para que el centro de la base del cono
    // esté en el origen del rayo punto p)
    Point p0(p.getCoord()[0] - vertex.getCoord()[0],
             p.getCoord()[1] - vertex.getCoord()[1],
             p.getCoord()[2] - vertex.getCoord()[2]);

    float a = cos(theta) * D.getCoord()[0] * D.getCoord()[0] +
              cos(theta) * D.getCoord()[2] * D.getCoord()[2] -
              sin(theta) * D.getCoord()[1] * D.getCoord()[1];

    float b = cos(theta) * D.getCoord()[0] * p0.getCoord()[0] +
              cos(theta) * D.getCoord()[2] * p0.getCoord()[2] -
              sin(theta) * D.getCoord()[1] * p0.getCoord()[1];

    float c = cos(theta) * p0.getCoord()[0] * p0.getCoord()[0] +
              cos(theta) * p0.getCoord()[2] * p0.getCoord()[2] -
              sin(theta) * p0.getCoord()[1] * p0.getCoord()[1];

    //No existen soluciones si el discriminante es negativo
    float d = b * b - a * c;
    if (d < 0)
        return false;

    float x1 = (-b + sqrt(d)) / a;
    float x2 = (-b - sqrt(d)) / a;

    if (x1 > x2)
        t = x2;
    if (t < 0)
        t = x1;
    if (t < 0)
        return false;

    float y = p0.getCoord()[1] + t * D.getCoord()[1];

    if (y < -h || y > 0)
        return false;
    return true;
}

void Cone::get_uv(const Direction &n, const float h, float &u, float &v)
{
    u = atan2(n.getCoord()[0], n.getCoord()[2]) / (2 * M_PI) + 0.5;
    v = 1 - (h / this->h);
}