/****************************************+
 * Fichero: Geometry.cpp
 * Autor: David Solanas
 *****************************************/

#include "Geometry.hpp"
#include <cmath>
#include <iostream>
#include <random>

Point::Point() {}

/**
 * Constructor de la clase Point, recibe como parámetros
 * las 3 coordenadas xyz
 */
Point::Point(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->d = 1;
}

/**
 * Suma de un punto con un vector dirección
 * devuelve como resultado un punto
 */
const Point Point::operator+(const Direction &d) const
{
    return Point(this->x + d.x, this->y + d.y, this->z + d.z);
}

/**
 * Resta de un punto con otro punto, devuelve
 * como resultado un vector dirección
 */
const Direction Point::operator-(const Point &p) const
{
    return Direction(this->x - p.x, this->y - p.y, this->z - p.z);
}

/**
 * Copia el punto p al punto actual
 */
Point &Point::operator=(const Point &p)
{
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
    this->d = p.d;
    return *this;
}

/**
 * Copia las coordenadas especificadas al punto
 */
void Point::setCoord(float _x, float _y, float _z, float _d)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
    this->d = _d;
}

Direction::Direction() {}

/**
 * Constructor de la clase Direction, recibe
 * como parámetros las tres coordenadas xyz
 */
Direction::Direction(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->d = 0;
}

/**
 * Producto de un vector por un escalar, devuelve un vector
 */
const Direction Direction::operator*(const float s) const
{
    return Direction(x * s, y * s, z * s);
}

/**
 * Suma de dos vectores, devuelve un vector
 */
const Direction Direction::operator+(const Direction &d) const
{
    return Direction(this->x + d.x, this->y + d.y, this->z + d.z);
}

/**
 * Suma de un vector con un punto, devuelve un punto
 */
const Point Direction::operator+(const Point &p) const
{
    return Point(this->x + p.x, this->y + p.y, this->z + p.z);
}

/**
 * Resta de dos vectores, devuelve un vector
 */
const Direction Direction::operator-(const Direction &d) const
{
    return Direction(this->x - d.x, this->y - d.y, this->z - d.z);
}

/**
 * División de un vector por un escalar, devuelve un vector
 */
const Direction Direction::operator/(const float s) const
{
    return Direction(x / s, y / s, z / s);
}

/**
* Copia el punto p al punto actual
*/
Direction &Direction::operator=(const Direction &d)
{
    this->x = d.x;
    this->y = d.y;
    this->z = d.z;
    this->d = d.d;
    return *this;
}

/**
 * Copia las coordenadas especificadas al vector
 */
void Direction::setCoord(float _x, float _y, float _z, float _d)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
    this->d = _d;
}

/**
 * Devuelve el módulo del vector dirección
 */
const float Direction::mod() const
{
    return std::sqrt(x * x + y * y + z * z);
}

/**
 * Producto escalar entre dos vectores dirección,
 * devuelve como resultado un escalar
 */
const float dot(const Direction &d1, const Direction &d2)
{
    return d1.x * d2.x + d1.y * d2.y + d1.z * d2.z;
}

/**
 * Devuelve el ángulo entre dos vectores en radianes
 */
const float get_angle(const Direction &d1, const Direction &d2)
{
    return acos(dot(d1, d2) / (d1.mod() * d2.mod()));
}

/**
 * Producto escalar entre un vector dirección y un punto
 * devuelve como resultado un escalar
 */
const float dot(const Direction &d, const Point &p)
{

    return d.x * p.x + d.y * p.y + d.z * p.z;
}

/**
 * Producto vectorial entre dos vectores dirección, devuelve
 * como resultado un vector dirección perpendicular a los dos
 * vectores
 */
const Direction cross(const Direction &d1, const Direction &d2)
{
    return Direction(d1.y * d2.z - d1.z * d2.y,
                     d1.z * d2.x - d1.x * d2.z,
                     d1.x * d2.y - d1.y * d2.x);
}

/**
 * Normaliza el vector d
 */
const Direction normalize(const Direction &d)
{
    float mod = d.mod();
    return Direction(d.x / mod, d.y / mod, d.z / mod);
}

void Point::view()
{
    std::cout << "<" << x << ", " << y << ", " << z << ", " << d << ">" << std::endl;
}

void Direction::view()
{
    std::cout << "<" << x << ", " << y << ", " << z << ", " << d << ">" << std::endl;
}

Point get_random_point()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist;
    float x = dist(mt),
          y = dist(mt),
          z = dist(mt);
    return Point(x, y, z);
}

Direction get_random_vect()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist;
    float x = dist(mt),
          y = dist(mt),
          z = dist(mt);
    return Direction(x, y, z);
}