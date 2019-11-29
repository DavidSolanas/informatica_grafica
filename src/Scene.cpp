/****************************************+
 * Fichero: Scene.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Scene.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include <cmath>

Camera::Camera(const Direction &_f, const Direction &_u, const Direction &_l, const Point &_o)
{
    this->f = _f;
    this->u = _u;
    this->l = _l;
    this->o = _o;
}

Camera::Camera(const Point &_o, const Direction &_f)
{
}

Direction Camera::get_f()
{
    return this->f;
}

Direction Camera::get_u()
{
    return this->u;
}

Direction Camera::get_l()
{
    return this->l;
}

Point Camera::get_o()
{
    return this->o;
}

World::World()
{
}

/// Set & get background color.
void World::set_background(const RGB bkg)
{
    this->background = bkg;
}

RGB World::get_background()
{
    return this->background;
}

/// Add a light source to our World
void World::add_light(Light *ls)
{
    this->light_list.push_back(ls);
}

Light *World::light(const int idx) const
{
    return this->light_list[idx];
}

int World::n_lights() const
{
    return this->light_list.size();
}

/// Add an object to our world
void World::add_object(Geometry *o3d)
{
    this->object_list.push_back(o3d);
}

void World::add_objects(std::vector<Geometry *> &o3ds)
{
    this->object_list = o3ds;
}

/// Return the object that first intersects `ray'
Geometry *World::first_intersection(Ray &ray)
{
    int closest_idx = -1;
    float tmin = INFINITY;
    for (int j = 0; j < object_list.size(); j++)
    {
        if (object_list[j]->intersect(ray))
        {
            if (ray.get_parameter() > 0 && ray.get_parameter() < tmin)
            {
                closest_idx = j;
                tmin = ray.get_parameter();
            }
        }
    }
    if (closest_idx == -1)
    {
        return nullptr;
    }
    //Ha habido intersección
    ray.set_parameter(tmin);
    return object_list[closest_idx];
}

/// Return the light that first intersects `ray'
float World::get_incoming_light(const Point &X)
{
    return .0f;
}

std::vector<Geometry *> scene1(Camera c, const int W, const int H)
{
    std::vector<Geometry *> geometry;
    float split = W / 5;
    for (int i = 0; i < 5; i++)
    {
        geometry.push_back(new Sphere(Point((i + 1) * split - (split / 2), (H / 2 - 200) + i * 75, c.f.mod() + i * 100), Direction(0, 200, 0),
                                      Point((i + 1) * split - (split / 2) + 100, (H / 2 - 200) + i * 75, c.f.mod() + i * 100)));
    }
    geometry.push_back(new Plane(Direction(0, 1, 0), Point(W / 2, c.o.y - c.u.mod(), c.f.mod())));
    geometry.push_back(new Plane(Direction(0, -1, 0), Point(W / 2, c.o.y + c.u.mod(), c.f.mod())));
    geometry.push_back(new Plane(Direction(1, 0, 0), Point(c.o.x - c.l.mod(), H / 2, c.f.mod())));
    geometry.push_back(new Plane(Direction(-1, 0, 0), Point(c.o.x + c.l.mod(), H / 2, c.f.mod())));
    geometry.push_back(new Plane(Direction(0, 0, -1), Point(W / 2, H / 2, c.f.mod() + 750)));
    // CARA FRONTAL
    geometry.push_back(new BoundedPlane(
        Point(500, H - 100, c.f.mod() + 300),
        Point(800, H - 100, c.f.mod() + 300),
        Point(800, H - 300, c.f.mod() + 300),
        Point(500, H - 300, c.f.mod() + 300)));

    // CARA DERECHA
    geometry.push_back(new BoundedPlane(
        Point(800, H - 100, c.f.mod() + 300),
        Point(900, H - 100, c.f.mod() + 400),
        Point(900, H - 300, c.f.mod() + 400),
        Point(800, H - 300, c.f.mod() + 300)));
    //CARA IZQUIERDA
    geometry.push_back(new BoundedPlane(
        Point(500, H - 100, c.f.mod() + 300),
        Point(600, H - 100, c.f.mod() + 400),
        Point(600, H - 300, c.f.mod() + 400),
        Point(500, H - 300, c.f.mod() + 300)));
    //CARA TRASERA
    geometry.push_back(new BoundedPlane(
        Point(600, H - 100, c.f.mod() + 400),
        Point(900, H - 100, c.f.mod() + 400),
        Point(900, H - 300, c.f.mod() + 400),
        Point(600, H - 300, c.f.mod() + 400)));
    // CARA SUPERIOR
    geometry.push_back(new BoundedPlane(
        Point(500, H - 100, c.f.mod() + 300),
        Point(600, H - 100, c.f.mod() + 400),
        Point(900, H - 100, c.f.mod() + 400),
        Point(800, H - 100, c.f.mod() + 300)));
    //CARA INFERIOR
    geometry.push_back(new BoundedPlane(
        Point(500, H - 300, c.f.mod() + 300),
        Point(600, H - 300, c.f.mod() + 400),
        Point(900, H - 300, c.f.mod() + 400),
        Point(800, H - 300, c.f.mod() + 300)));

    geometry.push_back(new Triangle(
        Point(500, H - 100, c.f.mod() + 299),
        Point(800, H - 300, c.f.mod() + 299),
        Point(500, H - 300, c.f.mod() + 299)));
    return geometry;
}

std::vector<Geometry *> scene2(Camera c, const int W, const int H)
{
    std::vector<Geometry *> geometry;

    //Pared IZQ
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod()),
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod())));

    //Pared DCH
    geometry.push_back(new BoundedPlane(
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750)));

    //Pared Fondo
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750)));

    //Pared Superior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750)));

    //Pared Inferior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod())));

    //Lado IZQ
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 - 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 - 50, 0, c.f.mod() + 100),
        Point(W / 2 - 50, 0, c.f.mod())));

    //Lado DCH
    geometry.push_back(new BoundedPlane(
        Point(W / 2 + 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, 0, c.f.mod()),
        Point(W / 2 + 50, 0, c.f.mod() + 100)));

    //Lado Fondo
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 + 50, 0, c.f.mod() + 100),
        Point(W / 2 - 50, 0, c.f.mod() + 100)));

    //Lado frontal
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, 0, c.f.mod()),
        Point(W / 2 - 50, 0, c.f.mod())));

    //Lado Superior
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 - 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod() + 100)));

    //Lado Inferior
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, 0, c.f.mod()),
        Point(W / 2 - 50, 0, c.f.mod() + 100),
        Point(W / 2 + 50, 0, c.f.mod() + 100),
        Point(W / 2 + 50, 0, c.f.mod())));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.f.mod() + 50)));

    return geometry;
}

std::vector<Geometry *> scene3(Camera c, const int W, const int H)
{
    std::vector<Geometry *> geometry;

    //Pared IZQ
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod()),
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod())));

    //Pared DCH
    geometry.push_back(new BoundedPlane(
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750)));

    //Pared Fondo
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750)));

    //Pared Superior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750)));

    //Pared Inferior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod())));

    //Cilindro
    geometry.push_back(new Cylinder(
        Disk(Direction(0, -1, 0), Point(W / 2, 0, c.f.mod() + 50), 50),
        Disk(Direction(0, 1, 0), Point(W / 2, H / 2 - 50, c.f.mod() + 50), 50),
        50, H / 2 - 50));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.f.mod() + 50)));

    return geometry;
}

std::vector<Geometry *> scene4(Camera c, const int W, const int H)
{
    std::vector<Geometry *> geometry;
    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod()), Direction(0, 200, 0),
        Point(W / 2 - 100, H / 2, c.f.mod())));

    return geometry;
}

std::vector<Geometry *> scene5(Camera c, const int W, const int H)
{
    std::vector<Geometry *> geometry;

    //Pared IZQ
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod()),
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod())));

    //Pared DCH
    geometry.push_back(new BoundedPlane(
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750)));

    //Pared Fondo
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750)));

    //Pared Superior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750)));

    //Pared Inferior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod())));

    //Cilindro
    geometry.push_back(new Cylinder(
        Disk(Direction(0, -1, 0), Point(W / 2, 0, c.f.mod() + 50), 50),
        Disk(Direction(0, 1, 0), Point(W / 2, H / 2 - 50, c.f.mod() + 50), 50),
        50, H / 2 - 50));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.f.mod() + 50)));

    // Cono
    geometry.push_back(new Cone(
        Point(W / 2 - 500, H / 2 - 200, c.f.mod() + 400),
        300, 150));

    geometry.push_back(new Disk(
        Direction(0, 0, -1),
        Point(W / 2 + 500, H / 2 - 200, c.f.mod() + 400),
        55.5f));

    return geometry;
}