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

Cone::Cone(const Point &p, float h, float r, const RGB &c, const float ior) : Object(c)
{
    idx_of_refraction = ior;
    this->vertex = p;
    this->h = h;
    this->r = r;
    //Calcular el angulo de la parte superior del cono,
    //definir el punto superior y un punto en un extremo de
    //la base del cono y calcular el arccos(altura/lado)
    Point p1(p.x + r, p.y, p.z);
    Point p2(p.x, p.y - h, p.z);
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
    return vertex.y;
}

Direction Cone::getNormal(Point X)
{
    //Punto central a altura Xy
    Point p1(vertex.x, X.y, vertex.z);
    //Punto superior
    Point p2(vertex.x, vertex.y + h, vertex.z);

    Direction w = X - vertex;
    w.y = 0.0f;
    w = normalize(w);

    return Direction(w.x * h / r, r / h, w.z * h / r);
}

bool Cone::intersect(Ray &ray)
{
    // Trasladamos el rayo para que el centro de la base del cono
    // esté en el origen del rayo punto p)
    Point p0(ray.get_origin().x - vertex.x,
             ray.get_origin().y - vertex.y,
             ray.get_origin().z - vertex.z);

    float a = cos(theta) * ray.get_direction().x * ray.get_direction().x +
              cos(theta) * ray.get_direction().z * ray.get_direction().z -
              sin(theta) * ray.get_direction().y * ray.get_direction().y;

    float b = cos(theta) * ray.get_direction().x * p0.x +
              cos(theta) * ray.get_direction().z * p0.z -
              sin(theta) * ray.get_direction().y * p0.y;

    float c = cos(theta) * p0.x * p0.x +
              cos(theta) * p0.z * p0.z -
              sin(theta) * p0.y * p0.y;

    //No existen soluciones si el discriminante es negativo
    float d = b * b - a * c;
    if (d < 0)
        return false;

    float x1 = (-b + sqrt(d)) / a;
    float x2 = (-b - sqrt(d)) / a;
    float t = 0;
    if (x1 > x2)
        t = x2;
    if (t < 0)
        t = x1;
    if (t < 0)
        return false;

    float y = p0.y + t * ray.get_direction().y;

    if (y < -h || y > 0)
        return false;
    ray.set_parameter(t);
    return true;
}

float Cone::get_area()
{
    Point p1(vertex.x + r, vertex.y, vertex.z);
    Point p2(vertex.x, vertex.y - h, vertex.z);
    float l = (p2 - p1).mod();
    return (M_PI * r * r) + (M_PI * r * l);
}

void Cone::get_uv(const Direction &n, const float h, float &u, float &v)
{
    u = atan2(n.x, n.z) / (2 * M_PI) + 0.5;
    v = 1 - (h / this->h);
}