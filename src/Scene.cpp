/****************************************+
 * Fichero: Scene.cpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#include "Scene.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include <cmath>
#include "Lambertian.hpp"
#include "Phong.hpp"
#include "Specular.hpp"
#include "Transmissive.hpp"
#include "Material.hpp"
#include "Dielectric.hpp"
#include "Cube.hpp"

//Some lambertian colors
BRDF *white = new Lambertian(RGB(.85, .85, .85));
BRDF *brown = new Lambertian(RGB(.45, .23, .06));
BRDF *red = new Lambertian(RGB(.85, .085, .085));
BRDF *green = new Lambertian(RGB(.085, .85, .085));
BRDF *orange = new Lambertian(RGB(.85, .6, .0));
BRDF *orange_phong = new Phong(RGB(.425, .3, .0), RGB(0.35, 0.20, .0), 8.0f);
BRDF *blue = new Lambertian(RGB(.085, .085, .85));
BRDF *blue_phong = new Phong(RGB(.085, .085, .425), RGB(0.085, 0.085, .35), 5.0f);
BRDF *mirror = new Specular(RGB(.85, .85, .85));
BRDF *glass = new Transmissive(RGB(.85, .85, .85), GLASS_REFRACTION_INDEX);
BRDF *water = new Transmissive(RGB(.85, .85, .85), WATER_REFRACTION_INDEX);
BRDF *dielectric = new Dielectric(RGB(.45, .45, .45), RGB(.45, .45, .45), GLASS_REFRACTION_INDEX);
BRDF *test = new Material(RGB(.085, .2, .4), RGB(.15, .15, .35), RGB(.15, .15, .15), RGB(.0, .0, .0), 4, AIR_REFRACTION_INDEX);

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
        Ld = Ld + light->get_incoming_light(*this, X, hit_normal);
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

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2, H / 2, c.f.mod() + 375), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.f.mod() + 375),
        mirror));

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
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    //Pared DCH
    geometry.push_back(new BoundedPlane(
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), H, c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        white));

    //Pared Fondo
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), H, c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), H, c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        white));

    //Pared Superior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 1500),
        white));

    //Pared Inferior
    geometry.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white, true, "../SmallPM_v1/SmallPM-linux/SmallPM/data/textures/wood_floor.ppm"));

    //Cilindro
    geometry.push_back(new Cylinder(
        Disk(Direction(0, -1, 0), Point(W / 2, 0, c.f.mod() + 600), 50, white),
        Disk(Direction(0, 1, 0), Point(W / 2, H / 2 - 50, c.f.mod() + 600), 50, white),
        50, H / 2 - 50, white, true, "../SmallPM_v1/SmallPM-linux/SmallPM/data/textures/marmol_azul.ppm"));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2 + 150, H / 2 + 60, c.f.mod() + 700), Direction(0, 150, 0),
        Point(W / 2 + 225, H / 2 + 60, c.f.mod() + 700),
        test));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2 + 200, 75, c.f.mod() + 500), Direction(0, 110, 0),
        Point(W / 2 + 255, 75, c.f.mod() + 500),
        glass));

    // Esfera
    geometry.push_back(new Sphere(
        Point(W / 2 - 150, H / 2 - 100, c.f.mod() + 1000), Direction(0, 150, 0),
        Point(W / 2 - 225, H / 2 - 100, c.f.mod() + 1000),
        white, true, "../SmallPM_v1/SmallPM-linux/SmallPM/data/textures/marmol.ppm"));

    // Cono
    geometry.push_back(new Cone(
        Point(W / 2, H / 2 + 100, c.f.mod() + 600),
        150, 15, white, true, "../SmallPM_v1/SmallPM-linux/SmallPM/data/textures/chess.ppm"));

    // CARA FRONTAL
    geometry.push_back(new BoundedPlane(
        Point(0, H / 2, c.f.mod() + 200),
        Point(5, H / 2 + 5, c.f.mod() + 200),
        Point(45, 5, c.f.mod() + 200),
        Point(40, 0, c.f.mod() + 200),
        brown));
    //CARA TRASERA
    geometry.push_back(new BoundedPlane(
        Point(0, H / 2, c.f.mod() + 900),
        Point(5, H / 2 + 5, c.f.mod() + 900),
        Point(45, 5, c.f.mod() + 900),
        Point(40, 0, c.f.mod() + 900),
        brown));
    //CARA IZQUIERDA
    geometry.push_back(new BoundedPlane(
        Point(0, H / 2, c.f.mod() + 900),
        Point(0, H / 2, c.f.mod() + 200),
        Point(40, 0, c.f.mod() + 200),
        Point(40, 0, c.f.mod() + 900),
        brown));
    // CARA DERECHA
    geometry.push_back(new BoundedPlane(
        Point(5, H / 2 + 5, c.f.mod() + 200),
        Point(5, H / 2 + 5, c.f.mod() + 900),
        Point(45, 5, c.f.mod() + 900),
        Point(45, 5, c.f.mod() + 200),
        mirror));
    // CARA SUPERIOR
    geometry.push_back(new BoundedPlane(
        Point(0, H / 2, c.f.mod() + 900),
        Point(5, H / 2 + 5, c.f.mod() + 900),
        Point(5, H / 2 + 5, c.f.mod() + 200),
        Point(0, H / 2, c.f.mod() + 200),
        brown));
    //CARA INFERIOR
    geometry.push_back(new BoundedPlane(
        Point(40, 0, c.f.mod() + 200),
        Point(45, 5, c.f.mod() + 200),
        Point(45, 5, c.f.mod() + 900),
        Point(40, 0, c.f.mod() + 900),
        brown));

    //***************************************************
    // CARA FRONTAL
    geometry.push_back(new BoundedPlane(
        Point(W - 5, H / 2 + 75, c.f.mod() + 300),
        Point(W, H / 2 + 75, c.f.mod() + 300),
        Point(W, H / 2 - 75, c.f.mod() + 300),
        Point(W - 5, H / 2 - 75, c.f.mod() + 300),
        brown));
    //CARA TRASERA
    geometry.push_back(new BoundedPlane(
        Point(W - 5, H / 2 + 75, c.f.mod() + 700),
        Point(W, H / 2 + 75, c.f.mod() + 700),
        Point(W, H / 2 - 75, c.f.mod() + 700),
        Point(W - 5, H / 2 - 75, c.f.mod() + 700),
        brown));
    //CARA IZQUIERDA
    geometry.push_back(new BoundedPlane(
        Point(W - 5, H / 2 + 75, c.f.mod() + 700),
        Point(W - 5, H / 2 + 75, c.f.mod() + 300),
        Point(W - 5, H / 2 - 75, c.f.mod() + 300),
        Point(W - 5, H / 2 - 75, c.f.mod() + 700),
        white, true, "../SmallPM_v1/SmallPM-linux/SmallPM/data/textures/noche-estrellada.ppm"));
    // CARA DERECHA,
    geometry.push_back(new BoundedPlane(
        Point(W, H / 2 + 75, c.f.mod() + 300),
        Point(W, H / 2 + 75, c.f.mod() + 700),
        Point(W, H / 2 - 75, c.f.mod() + 700),
        Point(W, H / 2 - 75, c.f.mod() + 300),
        brown));
    // CARA SUPERIOR
    geometry.push_back(new BoundedPlane(
        Point(W - 5, H / 2 + 75, c.f.mod() + 700),
        Point(W, H / 2 + 75, c.f.mod() + 700),
        Point(W, H / 2 + 75, c.f.mod() + 300),
        Point(W - 5, H / 2 + 75, c.f.mod() + 300),
        brown));
    //CARA INFERIOR
    geometry.push_back(new BoundedPlane(
        Point(W - 5, H / 2 - 75, c.f.mod() + 300),
        Point(W, H / 2 - 75, c.f.mod() + 300),
        Point(W, H / 2 - 75, c.f.mod() + 700),
        Point(W - 5, H / 2 - 75, c.f.mod() + 700),
        brown));

    //***************************************************
    // CARA FRONTAL
    geometry.push_back(new BoundedPlane(
        Point(50, H - 75, c.f.mod() + 1495),
        Point(590, H - 75, c.f.mod() + 1495),
        Point(590, 75, c.f.mod() + 1495),
        Point(50, 75, c.f.mod() + 1495),
        white, true, "../SmallPM_v1/SmallPM-linux/SmallPM/data/textures/guernica.ppm"));
    //CARA TRASERA
    geometry.push_back(new BoundedPlane(
        Point(50, H - 75, c.f.mod() + 1500),
        Point(590, H - 75, c.f.mod() + 1500),
        Point(590, 75, c.f.mod() + 1500),
        Point(50, 75, c.f.mod() + 1500),
        brown));
    //CARA IZQUIERDA
    geometry.push_back(new BoundedPlane(
        Point(50, H - 75, c.f.mod() + 1500),
        Point(50, H - 75, c.f.mod() + 1495),
        Point(50, 75, c.f.mod() + 1495),
        Point(50, 75, c.f.mod() + 1500),
        brown));
    // CARA DERECHA
    geometry.push_back(new BoundedPlane(
        Point(590, H - 75, c.f.mod() + 1495),
        Point(590, H - 75, c.f.mod() + 1500),
        Point(590, 75, c.f.mod() + 1500),
        Point(590, 75, c.f.mod() + 1495),
        brown));
    // CARA SUPERIOR
    geometry.push_back(new BoundedPlane(
        Point(50, H - 75, c.f.mod() + 1500),
        Point(590, H - 75, c.f.mod() + 1500),
        Point(590, H - 75, c.f.mod() + 1495),
        Point(50, H - 75, c.f.mod() + 1495),
        brown));
    //CARA INFERIOR
    geometry.push_back(new BoundedPlane(
        Point(50, 75, c.f.mod() + 1495),
        Point(590, 75, c.f.mod() + 1495),
        Point(590, 75, c.f.mod() + 1500),
        Point(50, 75, c.f.mod() + 1500),
        brown));

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
        Point(W / 2 - 125, 115, c.f.mod() + 1150), Direction(0, 230, 0),
        Point(W / 2 - 10, 115, c.f.mod() + 1150), mirror));

    // Esfera
    objects.push_back(new Sphere(
        Point(W / 2 + 150, 150, c.f.mod() + 400), Direction(0, 230, 0),
        Point(W / 2 + 35, 150, c.f.mod() + 400),
        glass));

    return objects;
}

std::vector<Object *> cornell_box_test(Camera c, const int W, const int H)
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
        orange_phong));

    // Esfera
    objects.push_back(new Sphere(
        Point(W / 2, 75, c.f.mod() + 500), Direction(0, 150, 0),
        Point(W / 2 + 75, 75, c.f.mod() + 500),
        water));

    // Esfera
    objects.push_back(new Sphere(
        Point(W / 2 + 200, 150, c.f.mod() + 750), Direction(0, 150, 0),
        Point(W / 2 + 125, 150, c.f.mod() + 750),
        dielectric));

    // Esfera
    objects.push_back(new Sphere(
        Point(75, 240, c.f.mod() + 900), Direction(0, 150, 0),
        Point(0, 240, c.f.mod() + 900),
        test));

    return objects;
}

std::vector<Object *> texture_test(Camera c, const int W, const int H)
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
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod()),
        Point(c.o.x + c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x - c.l.mod(), c.o.y + c.u.mod(), c.f.mod() + 1500),
        white));

    //Pared Inferior
    objects.push_back(new BoundedPlane(
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod() + 1500),
        Point(c.o.x + c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        Point(c.o.x - c.l.mod(), c.o.y - c.u.mod(), c.f.mod()),
        white));

    return objects;
}
