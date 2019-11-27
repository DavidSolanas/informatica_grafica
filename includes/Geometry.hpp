/****************************************+
 * Fichero: Geometry.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <array>

class Point;
class Direction;

class Geometry
{
public:
    Geometry(){};
    virtual ~Geometry(){};
    virtual Direction getNormal(Point X) = 0;
    virtual bool intersect(const Point &p, const Direction &D, float &t) = 0;
};

/**
 * Clase punto representado por 4 coordenadas en el espacio
 * tridimensional, la cuarta coordenada indica que es un punto
 * (valor 1)
 */
class Point
{
private:
    /**
     * Vector de coordenadas del punto
     */
    std::array<float, 4> c;

public:
    Point();
    /**
     * Constructor de la clase Point, recibe como parámetros
     * las 3 coordenadas xyz
     */
    Point(float x, float y, float z);

    /**
     * Suma de un punto con un vector dirección
     * devuelve como resultado un punto
     */
    const Point operator+(const Direction &d) const;

    /**
     * Resta de un punto con otro punto, devuelve
     * como resultado un vector dirección
     */
    const Direction operator-(const Point &p) const;

    /**
    * Copia el punto p al punto actual
    */
    Point &operator=(const Point &p);

    /**
     * Devuelve las coordenada  del punto
     */
    const std::array<float, 4> getCoord() const;

    /**
     * Copia las coordenadas especificadas al punto
     */
    void setCoord(std::array<float, 4> c);

    void view();
};

/**
 * Clase vector dirección representado por 4 coordenadas en el espacio
 * tridimensional, la cuarta coordenada indica que es una dirección
 * (valor 0)
 */
class Direction
{
private:
    /**
     * Vector de coordenadas del vector dirección
     */
    std::array<float, 4> c;

public:
    Direction();
    /**
     * Constructor de la clase Direction, recibe
     * como parámetros las tres coordenadas xyz
     */
    Direction(float x, float y, float z);

    /**
     * Producto de un vector por un escalar, devuelve un vector
     */
    const Direction operator*(const float s) const;

    /**
     * Suma de dos vectores, devuelve un vector
     */
    const Direction operator+(const Direction &d) const;

    /**
     * Suma de un vector con un punto, devuelve un punto
     */
    const Point operator+(const Point &p) const;

    /**
     * Resta de dos vectores, devuelve un vector
     */
    const Direction operator-(const Direction &d) const;

    /**
     * División de un vector por un escalar, devuelve un vector
     */
    const Direction operator/(const float s) const;

    /**
    * Copia la dirección d al vector dirección actual
    */
    Direction &operator=(const Direction &d);

    /**
     * Devuelve las coordenada  del vector
     */
    const std::array<float, 4> getCoord() const;

    /**
     * Copia las coordenadas especificadas al vector
     */
    void setCoord(std::array<float, 4> c);

    /**
     * Devuelve el módulo del vector dirección
     */
    const float mod() const;

    void view();
};

/**
 * Producto escalar entre dos vectores dirección,
 * devuelve como resultado un escalar
 */
const float dot(const Direction &d1, const Direction &d2);

/**
 * Devuelve el ángulo entre dos vectores en radianes
 */
const float get_angle(const Direction &d1, const Direction &d2);

/**
 * Producto escalar entre un vector dirección y un punto
 * devuelve como resultado un escalar
 */
const float dot(const Direction &d, const Point &p);

/**
 * Producto vectorial entre dos vectores dirección, devuelve
 * como resultado un vector dirección perpendicular a los dos
 * vectores
 */
const Direction cross(const Direction &d1, const Direction &d2);

/**
 * Normaliza el vector d
 */
const Direction normalize(const Direction &d);

Point get_random_point();

Direction get_random_vect();

#endif // !GEOMETRY_HPP