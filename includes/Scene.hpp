/****************************************+
 * Fichero: Scene.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Geometry.hpp"

class Camera
{
public:
    Direction f;
    Direction u;
    Direction l;
    Point o;

    Camera(const Direction &_f, const Direction &_u, const Direction &_l, const Point &_o);
    Camera(const Point &_o, const Direction &_f);
    Direction getF();
    Direction getU();
    Direction getL();
    Point getO();
};

std::array<std::unique_ptr<Geometry>, 17> scene1(Camera c, const int W, const int H);

std::array<std::unique_ptr<Geometry>, 12> scene2(Camera c, const int W, const int H);

std::array<std::unique_ptr<Geometry>, 7> scene3(Camera c, const int W, const int H);

std::array<std::unique_ptr<Geometry>, 1> scene4(Camera c, const int W, const int H);

std::array<std::unique_ptr<Geometry>, 9> scene5(Camera c, const int W, const int H);

#endif // !SCENE_HPP