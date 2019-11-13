
/****************************************+
 * Fichero: RGB.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef RGB_HPP
#define RGB_HPP

class RGB
{
private:
    float r, g, b;

public:
    RGB();
    RGB(float r, float g, float b);
    void setR(float r);
    void setG(float g);
    void setB(float b);
    float getR();
    float getG();
    float getB();
};

#endif