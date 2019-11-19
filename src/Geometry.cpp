/****************************************+
 * Fichero: Geometry.cpp
 * Autor: David Solanas
 *****************************************/

#include "Geometry.hpp"
#include <cmath>
#include <iostream>

Point::Point() {}

/**
 * Constructor de la clase Point, recibe como parámetros
 * las 3 coordenadas xyz
 */
Point::Point(float x, float y, float z)
{
    this->c[0] = x;
    this->c[1] = y;
    this->c[2] = z;
    this->c[3] = 1;
}

/**
 * Suma de un punto con un vector dirección
 * devuelve como resultado un punto
 */
const Point Point::operator+(const Direction &d) const
{
    std::array<float, 4> cd = d.getCoord();
    return Point(this->c[0] + cd[0], this->c[1] + cd[1], this->c[2] + cd[2]);
}

/**
 * Resta de un punto con otro punto, devuelve
 * como resultado un vector dirección
 */
const Direction Point::operator-(const Point &p) const
{
    std::array<float, 4> cp = p.getCoord();
    return Direction(this->c[0] - cp[0], this->c[1] - cp[1], this->c[2] - cp[2]);
}

/**
 * Copia el punto p al punto actual
 */
Point &Point::operator=(const Point &p)
{
    this->c[0] = p.c[0];
    this->c[1] = p.c[1];
    this->c[2] = p.c[2];
    this->c[3] = p.c[3];
    return *this;
}

/**
 * Devuelve las coordenada  del punto
 */
const std::array<float, 4> Point::getCoord() const
{
    return this->c;
}

/**
 * Copia las coordenadas especificadas al punto
 */
void Point::setCoord(std::array<float, 4> c)
{
    this->c = c;
}

Direction::Direction() {}

/**
 * Constructor de la clase Direction, recibe
 * como parámetros las tres coordenadas xyz
 */
Direction::Direction(float x, float y, float z)
{
    this->c[0] = x;
    this->c[1] = y;
    this->c[2] = z;
    this->c[3] = 0;
}

/**
 * Producto de un vector por un escalar, devuelve un vector
 */
const Direction Direction::operator*(const float s) const
{
    return Direction(this->c[0] * s, this->c[1] * s, this->c[2] * s);
}

/**
 * Suma de dos vectores, devuelve un vector
 */
const Direction Direction::operator+(const Direction &d) const
{
    std::array<float, 4> cd = d.getCoord();
    return Direction(this->c[0] + cd[0], this->c[1] + cd[1], this->c[2] + cd[2]);
}

/**
 * Suma de un vector con un punto, devuelve un punto
 */
const Point Direction::operator+(const Point &p) const
{
    std::array<float, 4> cp = p.getCoord();
    return Point(cp[0] + this->c[0], cp[1] + this->c[1], cp[2] + this->c[2]);
}

/**
 * Resta de dos vectores, devuelve un vector
 */
const Direction Direction::operator-(const Direction &d) const
{
    std::array<float, 4> cd = d.getCoord();
    return Direction(this->c[0] - cd[0], this->c[1] - cd[1], this->c[2] - cd[2]);
}

/**
 * División de un vector por un escalar, devuelve un vector
 */
const Direction Direction::operator/(const float s) const
{
    return Direction(this->c[0] / s, this->c[1] / s, this->c[2] / s);
}

/**
* Copia el punto p al punto actual
*/
Direction &Direction::operator=(const Direction &d)
{
    this->c[0] = d.c[0];
    this->c[1] = d.c[1];
    this->c[2] = d.c[2];
    this->c[3] = d.c[3];
    return *this;
}

/**
 * Devuelve las coordenada  del vector
 */
const std::array<float, 4> Direction::getCoord() const
{
    return this->c;
}

/**
 * Copia las coordenadas especificadas al vector
 */
void Direction::setCoord(std::array<float, 4> c)
{
    this->c = c;
}

/**
 * Devuelve el módulo del vector dirección
 */
const float Direction::mod() const
{
    return std::sqrt(this->c[0] * this->c[0] +
                     this->c[1] * this->c[1] +
                     this->c[2] * this->c[2]);
}

/**
 * Producto escalar entre dos vectores dirección,
 * devuelve como resultado un escalar
 */
const float dot(const Direction &d1, const Direction &d2)
{
    std::array<float, 4> c1 = d1.getCoord();
    std::array<float, 4> c2 = d2.getCoord();
    return c1[0] * c2[0] + c1[1] * c2[1] + c1[2] * c2[2];
}

/**
 * Producto escalar entre un vector dirección y un punto
 * devuelve como resultado un escalar
 */
const float dot(const Direction &d, const Point &p)
{
    std::array<float, 4> c1 = d.getCoord();
    std::array<float, 4> c2 = p.getCoord();
    return c1[0] * c2[0] + c1[1] * c2[1] + c1[2] * c2[2];
}

/**
 * Producto vectorial entre dos vectores dirección, devuelve
 * como resultado un vector dirección perpendicular a los dos
 * vectores
 */
const Direction cross(const Direction &d1, const Direction &d2)
{
    std::array<float, 4> c1 = d1.getCoord();
    std::array<float, 4> c2 = d2.getCoord();
    return Direction(c1[1] * c2[2] - c1[2] * c2[1],
                     c1[2] * c2[0] - c1[0] * c2[2],
                     c1[0] * c2[1] - c1[1] * c2[0]);
}

/**
 * Normaliza el vector d
 */
const Direction normalize(const Direction &d)
{
    float mod = d.mod();
    std::array<float, 4> c = d.getCoord();
    return Direction(c[0] / mod, c[1] / mod, c[2] / mod);
}

void Point::view()
{
    std::cout << "<" << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3] << ">" << std::endl;
}

void Direction::view()
{
    std::cout << "<" << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3] << ">" << std::endl;
}