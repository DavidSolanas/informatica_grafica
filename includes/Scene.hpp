/****************************************+
 * Fichero: Scene.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Geometry.hpp"
#include "Light.hpp"
#include <vector>

/// This is the refraction index of some materials / mediums.
const float AIR_REFRACTION_INDEX = 1.0f;
const float WATER_REFRACTION_INDEX = 1.333f;
const float GLASS_REFRACTION_INDEX = 1.52f;
const float SAPPHIRE_REFRACTION_INDEX = 1.77f;
const float DIAMOND_REFRACTION_INDEX = 2.42f;

class Camera
{
public:
    Direction f;
    Direction u;
    Direction l;
    Point o;

    Camera(const Direction &_f, const Direction &_u, const Direction &_l, const Point &_o);
    Camera(const Point &_o, const Direction &_f);
    Direction get_f();
    Direction get_u();
    Direction get_l();
    Point get_o();
};

class World
{
private:
    std::vector<Geometry *> object_list;
    std::vector<Light *> light_list;
    RGB background;

public:
    World();

    /// Set & get background color.
    void set_background(const RGB bkg);
    RGB get_background();

    /// Add a light source to our World
    void add_light(Light *ls);
    Light *light(const int idx) const;
    int n_lights() const;

    /// Add an object to our world
    void add_object(Geometry *o3d);
    void add_objects(std::vector<Geometry *> &o3ds);

    /// Return the object that first intersects `ray'
    Geometry *first_intersection(Ray &ray);

    /// Return the total ammount of light incoming the point from the light sources
    float get_incoming_light(const Point &X);
};

std::vector<Geometry *> scene1(Camera c, const int W, const int H);

std::vector<Geometry *> scene2(Camera c, const int W, const int H);

std::vector<Geometry *> scene3(Camera c, const int W, const int H);

std::vector<Geometry *> scene4(Camera c, const int W, const int H);

std::vector<Geometry *> scene5(Camera c, const int W, const int H);

#endif // !SCENE_HPP