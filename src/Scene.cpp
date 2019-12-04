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
#include "Lambertian.hpp"

//Some lambertian colors
BRDF *white = new Lambertian(RGB(.85, .85, .85));
BRDF *red = new Lambertian(RGB(.85, .085, .085));
BRDF *green = new Lambertian(RGB(.085, .85, .085));
BRDF *orange = new Lambertian(RGB(.85, .6, .0));

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

RGB World::get_background() const
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
void World::add_object(Object *o3d)
{
    this->object_list.push_back(o3d);
}

void World::add_objects(std::vector<Object *> &o3ds)
{
    this->object_list = o3ds;
}

/// Return the object that first intersects `ray'
Object *World::first_intersection(Ray &ray) const
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

/// Return the total ammount of light incoming the point from the light sources
RGB World::get_incoming_light(const Point &X, const Direction &hit_normal) const
{
    RGB Ld(0.0f, 0.0f, 0.0f);
    for (Light *light : light_list)
    {
        Ld = Ld + light->get_incoming_light(X, hit_normal);
    }
    return Ld;
}

/// Return the light that first intersects `ray'
Light *World::first_light_intersection(Ray &ray) const
{
    int closest_idx = -1;
    float tmin = INFINITY;

    for (int j = 0; j < light_list.size(); j++)
    {
        // Si obj es nullptr se trata de una luz puntual, no intersecta
        Object *obj = light_list[j]->get_object();
        if (obj != nullptr && obj->intersect(ray))
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
    return light_list[closest_idx];
}

std::vector<Object *> scene1(Camera c, const int W, const int H)
{
    std::vector<Object *> geometry;
    float split = W / 5;
    for (int i = 0; i < 5; i++)
    {
        geometry.push_back(new Sphere(Point((i + 1) * split - (split / 2), (H / 2 - 200) + i * 75, c.f.mod() + i * 100), Direction(0, 200, 0),
                                      Point((i + 1) * split - (split / 2) + 100, (H / 2 - 200) + i * 75, c.f.mod() + i * 100),
                                      white));
    }
    geometry.push_back(new Plane(Direction(0, 1, 0), Point(W / 2, c.o.y - c.u.mod(), c.f.mod()), white));
    geometry.push_back(new Plane(Direction(0, -1, 0), Point(W / 2, c.o.y + c.u.mod(), c.f.mod()), white));
    geometry.push_back(new Plane(Direction(1, 0, 0), Point(c.o.x - c.l.mod(), H / 2, c.f.mod()), white));
    geometry.push_back(new Plane(Direction(-1, 0, 0), Point(c.o.x + c.l.mod(), H / 2, c.f.mod()), white));
    geometry.push_back(new Plane(Direction(0, 0, -1), Point(W / 2, H / 2, c.f.mod() + 750), white));
    // CARA FRONTAL
    geometry.push_back(new BoundedPlane(
        Point(500, H - 100, c.f.mod() + 300),
        Point(800, H - 100, c.f.mod() + 300),
        Point(800, H - 300, c.f.mod() + 300),
        Point(500, H - 300, c.f.mod() + 300),
        white));

    // CARA DERECHA
    geometry.push_back(new BoundedPlane(
        Point(800, H - 100, c.f.mod() + 300),
        Point(900, H - 100, c.f.mod() + 400),
        Point(900, H - 300, c.f.mod() + 400),
        Point(800, H - 300, c.f.mod() + 300),
        white));
    //CARA IZQUIERDA
    geometry.push_back(new BoundedPlane(
        Point(500, H - 100, c.f.mod() + 300),
        Point(600, H - 100, c.f.mod() + 400),
        Point(600, H - 300, c.f.mod() + 400),
        Point(500, H - 300, c.f.mod() + 300),
        white));
    //CARA TRASERA
    geometry.push_back(new BoundedPlane(
        Point(600, H - 100, c.f.mod() + 400),
        Point(900, H - 100, c.f.mod() + 400),
        Point(900, H - 300, c.f.mod() + 400),
        Point(600, H - 300, c.f.mod() + 400),
        white));
    // CARA SUPERIOR
    geometry.push_back(new BoundedPlane(
        Point(500, H - 100, c.f.mod() + 300),
        Point(600, H - 100, c.f.mod() + 400),
        Point(900, H - 100, c.f.mod() + 400),
        Point(800, H - 100, c.f.mod() + 300),
        white));
    //CARA INFERIOR
    geometry.push_back(new BoundedPlane(
        Point(500, H - 300, c.f.mod() + 300),
        Point(600, H - 300, c.f.mod() + 400),
        Point(900, H - 300, c.f.mod() + 400),
        Point(800, H - 300, c.f.mod() + 300),
        white));

    geometry.push_back(new Triangle(
        Point(500, H - 100, c.f.mod() + 299),
        Point(800, H - 300, c.f.mod() + 299),
        Point(500, H - 300, c.f.mod() + 299),
        white));
    return geometry;
}

std::vector<Object *> scene2(Camera c, const int W, const int H)
{
    std::vector<Object *> geometry;

    //Pared IZQ
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod()),
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    //Pared DCH
    geometry.push_back(new BoundedPlane(
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Fondo
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Superior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Inferior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    //Lado IZQ
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 - 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 - 50, 0, c.f.mod() + 100),
        Point(W / 2 - 50, 0, c.f.mod()),
        white));

    //Lado DCH
    geometry.push_back(new BoundedPlane(
        Point(W / 2 + 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, 0, c.f.mod()),
        Point(W / 2 + 50, 0, c.f.mod() + 100),
        white));

    //Lado Fondo
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 + 50, 0, c.f.mod() + 100),
        Point(W / 2 - 50, 0, c.f.mod() + 100),
        white));

    //Lado frontal
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, 0, c.f.mod()),
        Point(W / 2 - 50, 0, c.f.mod()),
        white));

    //Lado Superior
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.f.mod() + 100),
        Point(W / 2 - 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod()),
        Point(W / 2 + 50, H / 2 - 50, c.f.mod() + 100),
        white));

    //Lado Inferior
    geometry.push_back(new BoundedPlane(
        Point(W / 2 - 50, 0, c.f.mod()),
        Point(W / 2 - 50, 0, c.f.mod() + 100),
        Point(W / 2 + 50, 0, c.f.mod() + 100),
        Point(W / 2 + 50, 0, c.f.mod()),
        white));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.f.mod() + 50),
        white));

    return geometry;
}

std::vector<Object *> scene3(Camera c, const int W, const int H)
{
    std::vector<Object *> geometry;

    //Pared IZQ
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod()),
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    //Pared DCH
    geometry.push_back(new BoundedPlane(
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Fondo
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Superior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Inferior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    //Cilindro
    geometry.push_back(new Cylinder(
        Disk(Direction(0, -1, 0), Point(W / 2, 0, c.f.mod() + 50), 50, white),
        Disk(Direction(0, 1, 0), Point(W / 2, H / 2 - 50, c.f.mod() + 50), 50, white),
        50, H / 2 - 50,
        white));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.f.mod() + 50),
        white));

    return geometry;
}

std::vector<Object *> scene4(Camera c, const int W, const int H)
{
    std::vector<Object *> geometry;
    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod()), Direction(0, 200, 0),
        Point(W / 2 - 100, H / 2, c.f.mod()),
        white));

    return geometry;
}

std::vector<Object *> scene5(Camera c, const int W, const int H)
{
    std::vector<Object *> geometry;

    //Pared IZQ
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod()),
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    //Pared DCH
    geometry.push_back(new BoundedPlane(
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Fondo
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Superior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 750),
        white));

    //Pared Inferior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 750),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    //Cilindro
    geometry.push_back(new Cylinder(
        Disk(Direction(0, -1, 0), Point(W / 2, 0, c.f.mod() + 50), 50, white),
        Disk(Direction(0, 1, 0), Point(W / 2, H / 2 - 50, c.f.mod() + 50), 50, white),
        50, H / 2 - 50, white));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.f.mod() + 50),
        white));

    // Cono
    geometry.push_back(new Cone(
        Point(W / 2 - 500, H / 2 - 200, c.f.mod() + 400),
        300, 150, white));

    geometry.push_back(new Disk(
        Direction(0, 0, -1),
        Point(W / 2 + 500, H / 2 - 200, c.f.mod() + 400),
        55.5f, white));

    return geometry;
}

std::vector<Object *> cornell_box(Camera c, const int W, const int H)
{
    std::vector<Object *> objects;

    //Pared IZQ
    objects.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod()),
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        red));

    //Pared DCH
    objects.push_back(new BoundedPlane(
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), H, c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        green));

    //Pared Fondo
    objects.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        white));

    //Pared Superior
    objects.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 1500),
        white));

    //Pared Inferior
    objects.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    // Esfera
    objects.push_back(new Sphere(
        Point(W / 2 - 200, 75, c.f.mod() + 1250), Direction(0, 150, 0),
        Point(W / 2 - 125, 75, c.f.mod() + 1250),
        orange));

    // Esfera
    objects.push_back(new Sphere(
        Point(W / 2 + 200, 75, c.f.mod() + 500), Direction(0, 150, 0),
        Point(W / 2 + 125, 75, c.f.mod() + 500),
        orange));

    return objects;
}