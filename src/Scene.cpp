/****************************************+
 * Fichero: Scene.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Scene.hpp"

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