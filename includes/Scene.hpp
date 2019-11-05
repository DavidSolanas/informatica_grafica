/****************************************+
 * Fichero: Scene.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Geometry.hpp"

class Camera
{
private:
    Direction f;
    Direction u;
    Direction l;
    Point o;

public:
    Camera(const Direction &_f, const Direction &_u, const Direction &_l, const Point &_o);
    Camera(const Point &_o, const Direction &_f);
    Direction getF();
    Direction getU();
    Direction getL();
    Point getO();
};

#endif // !SCENE_HPP