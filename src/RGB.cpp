
/****************************************+
 * Fichero: RGB.hpp
 * Autor: David Solanas Sanz    738630
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

float RGB::get_r() const
{
    return this->r;
}

float RGB::get_g() const
{
    return this->g;
}

float RGB::get_b() const
{
    return this->b;
}

float RGB::max() const
{
    if (r > g && r > b)
    {
        return r;
    }
    else if (g > r && g > b)
    {
        return g;
    }
    else
    {
        return b;
    }
}

const RGB RGB::operator*(const float f) const
{
    return RGB(this->r * f, this->g * f, this->b * f);
}

const RGB RGB::operator*(const RGB &c) const
{
    return RGB(this->r * c.r, this->g * c.g, this->b * c.b);
}

const RGB RGB::operator/(const float f) const
{
    return RGB(this->r / f, this->g / f, this->b / f);
}

const RGB RGB::operator+(const RGB &c) const
{
    return RGB(this->r + c.r, this->g + c.g, this->b + c.b);
}

const RGB RGB::operator+(const float f) const
{
    return RGB(this->r + f, this->g + f, this->b + f);
}

const bool RGB::operator==(const float v) const
{
    return r == v && g == v && b == v;
}