
/****************************************+
 * Fichero: RGB.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef RGB_HPP
#define RGB_HPP

class RGB
{
public:
    float r, g, b;

    RGB();
    RGB(float r, float g, float b);
    void set_r(float r);
    void set_g(float g);
    void set_b(float b);
    float get_r();
    float get_g();
    float get_b();
    const RGB operator*(const float f) const;
    const RGB operator*(const RGB &c) const;
    const RGB operator/(const float f) const;
    const RGB operator+(const float f) const;
    const RGB operator+(const RGB &c) const;
};

#endif