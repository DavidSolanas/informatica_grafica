
/****************************************+
 * Fichero: RGB.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "RGB.hpp"

RGB::RGB()
{
}

RGB::RGB(float rv, float gv, float bv) : r(rv), g(gv), b(bv)
{
}

void RGB::set_r(float r)
{
    this->r = r;
}

void RGB::set_g(float g)
{
    this->g = g;
}

void RGB::set_b(float b)
{
    this->b = b;
}

float RGB::get_r()
{
    return this->r;
}

float RGB::get_g()
{
    return this->g;
}

float RGB::get_b()
{
    return this->b;
}

const RGB RGB::operator*(const float f) const
{
    return RGB(r * f, g * f, b * f);
}

const RGB RGB::operator*(const RGB &c) const
{
    return RGB(r * c.r, g * c.g, b * c.b);
}

const RGB RGB::operator/(const float f) const
{
    return RGB(r / f, g / f, b / f);
}

const RGB RGB::operator+(const RGB &c) const
{
    return RGB(r + c.r, g + c.g, b + c.b);
}

const RGB RGB::operator+(const float f) const
{
    return RGB(r + f, g + f, b + f);
}
