/****************************************+
 * Fichero: Scene.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Scene.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"

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

Direction Camera::getF()
{
    return this->f;
}

Direction Camera::getU()
{
    return this->u;
}

Direction Camera::getL()
{
    return this->l;
}

Point Camera::getO()
{
    return this->o;
}

std::array<std::unique_ptr<Geometry>, 17> scene1(Camera c, const int W, const int H)
{
    std::array<std::unique_ptr<Geometry>, 17> geometry;
    float split = W / 5;
    for (int i = 0; i < 5; i++)
    {
        geometry[i] = std::unique_ptr<Geometry>(new Sphere(Point((i + 1) * split - (split / 2), (H / 2 - 200) + i * 75, c.getF().mod() + i * 100), Direction(0, 200, 0),
                                                           Point((i + 1) * split - (split / 2) + 100, (H / 2 - 200) + i * 75, c.getF().mod() + i * 100)));
    }
    geometry[5] = std::unique_ptr<Geometry>(new Plane(Direction(0, 1, 0), Point(W / 2, c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod())));
    geometry[8] = std::unique_ptr<Geometry>(new Plane(Direction(0, -1, 0), Point(W / 2, c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod())));
    geometry[6] = std::unique_ptr<Geometry>(new Plane(Direction(1, 0, 0), Point(c.getO().getCoord()[0] - c.getL().mod(), H / 2, c.getF().mod())));
    geometry[7] = std::unique_ptr<Geometry>(new Plane(Direction(-1, 0, 0), Point(c.getO().getCoord()[0] + c.getL().mod(), H / 2, c.getF().mod())));
    geometry[9] = std::unique_ptr<Geometry>(new Plane(Direction(0, 0, -1), Point(W / 2, H / 2, c.getF().mod() + 750)));
    // CARA FRONTAL
    geometry[10] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(500, H - 100, c.getF().mod() + 300),
        Point(800, H - 100, c.getF().mod() + 300),
        Point(800, H - 300, c.getF().mod() + 300),
        Point(500, H - 300, c.getF().mod() + 300)));

    // CARA DERECHA
    geometry[11] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(800, H - 100, c.getF().mod() + 300),
        Point(900, H - 100, c.getF().mod() + 400),
        Point(900, H - 300, c.getF().mod() + 400),
        Point(800, H - 300, c.getF().mod() + 300)));
    //CARA IZQUIERDA
    geometry[12] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(500, H - 100, c.getF().mod() + 300),
        Point(600, H - 100, c.getF().mod() + 400),
        Point(600, H - 300, c.getF().mod() + 400),
        Point(500, H - 300, c.getF().mod() + 300)));
    //CARA TRASERA
    geometry[13] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(600, H - 100, c.getF().mod() + 400),
        Point(900, H - 100, c.getF().mod() + 400),
        Point(900, H - 300, c.getF().mod() + 400),
        Point(600, H - 300, c.getF().mod() + 400)));
    // CARA SUPERIOR
    geometry[14] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(500, H - 100, c.getF().mod() + 300),
        Point(600, H - 100, c.getF().mod() + 400),
        Point(900, H - 100, c.getF().mod() + 400),
        Point(800, H - 100, c.getF().mod() + 300)));
    //CARA INFERIOR
    geometry[15] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(500, H - 300, c.getF().mod() + 300),
        Point(600, H - 300, c.getF().mod() + 400),
        Point(900, H - 300, c.getF().mod() + 400),
        Point(800, H - 300, c.getF().mod() + 300)));

    geometry[16] = std::unique_ptr<Geometry>(new Triangle(
        Point(500, H - 100, c.getF().mod() + 299),
        Point(800, H - 300, c.getF().mod() + 299),
        Point(500, H - 300, c.getF().mod() + 299)));
    return geometry;
}

std::array<std::unique_ptr<Geometry>, 12> scene2(Camera c, const int W, const int H)
{
    std::array<std::unique_ptr<Geometry>, 12> geometry;

    //Pared IZQ
    geometry[0] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod()),
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod())));

    //Pared DCH
    geometry[1] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750)));

    //Pared Fondo
    geometry[2] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750)));

    //Pared Superior
    geometry[3] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod() + 750)));

    //Pared Inferior
    geometry[4] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod())));

    //Lado IZQ
    geometry[5] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.getF().mod()),
        Point(W / 2 - 50, H / 2 - 50, c.getF().mod() + 100),
        Point(W / 2 - 50, 0, c.getF().mod() + 100),
        Point(W / 2 - 50, 0, c.getF().mod())));

    //Lado DCH
    geometry[6] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(W / 2 + 50, H / 2 - 50, c.getF().mod() + 100),
        Point(W / 2 + 50, H / 2 - 50, c.getF().mod()),
        Point(W / 2 + 50, 0, c.getF().mod()),
        Point(W / 2 + 50, 0, c.getF().mod() + 100)));

    //Lado Fondo
    geometry[7] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.getF().mod() + 100),
        Point(W / 2 + 50, H / 2 - 50, c.getF().mod() + 100),
        Point(W / 2 + 50, 0, c.getF().mod() + 100),
        Point(W / 2 - 50, 0, c.getF().mod() + 100)));

    //Lado frontal
    geometry[8] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.getF().mod()),
        Point(W / 2 + 50, H / 2 - 50, c.getF().mod()),
        Point(W / 2 + 50, 0, c.getF().mod()),
        Point(W / 2 - 50, 0, c.getF().mod())));

    //Lado Superior
    geometry[9] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(W / 2 - 50, H / 2 - 50, c.getF().mod() + 100),
        Point(W / 2 - 50, H / 2 - 50, c.getF().mod()),
        Point(W / 2 + 50, H / 2 - 50, c.getF().mod()),
        Point(W / 2 + 50, H / 2 - 50, c.getF().mod() + 100)));

    //Lado Inferior
    geometry[10] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(W / 2 - 50, 0, c.getF().mod()),
        Point(W / 2 - 50, 0, c.getF().mod() + 100),
        Point(W / 2 + 50, 0, c.getF().mod() + 100),
        Point(W / 2 + 50, 0, c.getF().mod())));

    // Esfera
    geometry[11] = std::unique_ptr<Geometry>(new Sphere(
        Point(W / 2, H / 2, c.getF().mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.getF().mod() + 50)));

    return geometry;
}

std::array<std::unique_ptr<Geometry>, 7> scene3(Camera c, const int W, const int H)
{
    std::array<std::unique_ptr<Geometry>, 7> geometry;

    //Pared IZQ
    geometry[0] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod()),
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod())));

    //Pared DCH
    geometry[1] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750)));

    //Pared Fondo
    geometry[2] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750)));

    //Pared Superior
    geometry[3] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod() + 750)));

    //Pared Inferior
    geometry[4] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod())));

    //Cilindro
    geometry[5] = std::unique_ptr<Geometry>(new Cylinder(
        Point(W / 2, 0, c.getF().mod() + 50),
        Point(W / 2, H / 2 - 50, c.getF().mod() + 50),
        50, H / 2 - 50));

    // Esfera
    geometry[6] = std::unique_ptr<Geometry>(new Sphere(
        Point(W / 2, H / 2, c.getF().mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.getF().mod() + 50)));

    return geometry;
}

std::array<std::unique_ptr<Geometry>, 1> scene4(Camera c, const int W, const int H)
{
    std::array<std::unique_ptr<Geometry>, 1> geometry;
    // Esfera
    geometry[0] = std::unique_ptr<Geometry>(new Sphere(
        Point(W / 2, H / 2, c.getF().mod()), Direction(0, 200, 0),
        Point(W / 2 - 100, H / 2, c.getF().mod())));

    return geometry;
}

std::array<std::unique_ptr<Geometry>, 8> scene5(Camera c, const int W, const int H)
{
    std::array<std::unique_ptr<Geometry>, 8> geometry;

    //Pared IZQ
    geometry[0] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod()),
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod())));

    //Pared DCH
    geometry[1] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750)));

    //Pared Fondo
    geometry[2] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), H, c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750)));

    //Pared Superior
    geometry[3] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] + c.getU().mod(), c.getF().mod() + 750)));

    //Pared Inferior
    geometry[4] = std::unique_ptr<Geometry>(new BoundedPlane(
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod()),
        Point(c.getO().getCoord()[0] - c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod() + 750),
        Point(c.getO().getCoord()[0] + c.getL().mod(), c.getO().getCoord()[1] - c.getU().mod(), c.getF().mod())));

    //Cilindro
    geometry[5] = std::unique_ptr<Geometry>(new Cylinder(
        Point(W / 2, 0, c.getF().mod() + 50),
        Point(W / 2, H / 2 - 50, c.getF().mod() + 50),
        50, H / 2 - 50));

    // Esfera
    geometry[6] = std::unique_ptr<Geometry>(new Sphere(
        Point(W / 2, H / 2, c.getF().mod() + 50), Direction(0, 100, 0),
        Point(W / 2 - 50, H / 2, c.getF().mod() + 50)));

    // Cono
    geometry[7] = std::unique_ptr<Geometry>(new Cone(
        Point(W / 2 - 500, H / 2 - 200, c.getF().mod() + 400),
        300, 150));

    return geometry;
}