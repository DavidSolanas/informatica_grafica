
/****************************************+
 * Fichero: Cube.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Cube.hpp"
#include <iostream>
#include <cmath>

Direction Cube::getNormal(Point X)
{
    if (front.isInPlane(X))
    {
        return front.getNormal();
    }
    if (back.isInPlane(X))
    {
        return back.getNormal();
    }
    if (left.isInPlane(X))
    {
        return left.getNormal();
    }
    if (right.isInPlane(X))
    {
        return right.getNormal();
    }
    if (up.isInPlane(X))
    {
        return up.getNormal();
    }
    if (down.isInPlane(X))
    {
        return down.getNormal();
    }
    return Direction(-1, -1, -1);
}

bool Cube::intersect(Ray &ray)
{
    Ray ray_front = ray;
    Ray ray_back = ray;
    Ray ray_left = ray;
    Ray ray_right = ray;
    Ray ray_up = ray;
    Ray ray_down = ray;
    bool fi = front.intersect(ray_front);
    bool bi = back.intersect(ray_back);
    bool li = left.intersect(ray_left);
    bool ri = right.intersect(ray_right);
    bool ui = up.intersect(ray_up);
    bool di = down.intersect(ray_down);

    if (!fi && !bi && !li && !ri && !ui && !di)
    {
        // No ha habido intersección con ninguna cara del cubo
        return false;
    }
    bool b[] = {fi, bi, li, ri, ui, di};
    float t[] = {
        ray_front.get_parameter(), ray_back.get_parameter(),
        ray_left.get_parameter(), ray_right.get_parameter(),
        ray_up.get_parameter(), ray_down.get_parameter()};
    int i;
    float tmin = INFINITY;
    for (i = 0; i < 6; i++)
    {
        if (b[i] && t[i] < tmin)
            tmin = t[i];
    }
    ray.set_parameter(tmin);
    return true;
}

float Cube::get_area()
{
    return 6 * front.get_area();
}

void Cube::get_uv(const Point &X, const Direction &n, float &u, float &v)
{
    Direction normals[] = {
        front.getNormal(), back.getNormal(),
        left.getNormal(), right.getNormal(),
        up.getNormal(), down.getNormal()};
    int i;
    for (i = 0; i < 6; i++)
    {
        if (n == normals[i])
            break;
    }
    switch (i)
    {
    case 0:
        front.get_uv(X, n, u, v);
        break;
    case 1:
        back.get_uv(X, n, u, v);
        break;
    case 2:
        left.get_uv(X, n, u, v);
        break;
    case 3:
        right.get_uv(X, n, u, v);
        break;
    case 4:
        up.get_uv(X, n, u, v);
        break;
    case 5:
        down.get_uv(X, n, u, v);
        break;

    default:
        break;
    }

    u = u > 1.0f ? 1.0f : u;
    v = v > 1.0f ? 1.0f : v;
}
