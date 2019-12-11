/****************************************+
 * Fichero: Geometry.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "BRDF.hpp"

class Ray;

/** This is a constant used to dismiss intersections very close to previous
		intersections. */
const float SMALLEST_DIST = 1e-6;

class Object
{
protected:
    BRDF *material;

public:
    Object() {}
    Object(BRDF *mat) : material(mat) {}
    virtual ~Object(){};
    virtual Direction getNormal(Point X) = 0;
    virtual bool intersect(Ray &ray) = 0;
    virtual float get_area() = 0;
    BRDF *get_material() { return material; }
};

/**
 * Clase punto representado por 4 coordenadas en el espacio
 * tridimensional, la cuarta coordenada indica que es un punto
 * (valor 1)
 */
class Point
{
public:
    /**
     * Vector de coordenadas del punto
     */
    float x, y, z;
    float d;

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
     * Devuelve verdad si un punto es igual a otro
     */
    const bool operator==(const Point &p) const;

    /**
    * Copia el punto p al punto actual
    */
    Point &operator=(const Point &p);

    /**
     * Copia las coordenadas especificadas al punto
     */
    void setCoord(float _x, float _y, float _z, float _d);

    void view() const;
};

/**
 * Clase vector dirección representado por 4 coordenadas en el espacio
 * tridimensional, la cuarta coordenada indica que es una dirección
 * (valor 0)
 */
class Direction
{
public:
    /**
     * Vector de coordenadas del vector dirección
     */
    float x, y, z;
    float d;

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
     * Devuelve verdad si una dirección es igual a otra
     */
    const bool operator==(const Direction &d) const;

    /**
    * Copia la dirección d al vector dirección actual
    */
    Direction &operator=(const Direction &d);

    /**
     * Copia las coordenadas especificadas al vector
     */
    void setCoord(float _x, float _y, float _z, float _d);

    /**
     * Devuelve el módulo del vector dirección
     */
    const float mod() const;

    void view() const;
};

/** The Ray class is used to find intersections between a ray and the scene,
		but it also stores information. For instance the Ray remembers intersection
		points, and the refraction index of material it is passing through. */
class Ray
{
    /// Origin of the ray
    Point origin;

    /// The normalized direction of the ray
    Direction direction;

    /// The parameter -i.e. the distance we have traversed along the ray
    float t;

public:
    // Construct a ray. First argument is position. Second argument
    // is the direction of the ray. The magnitude of the second argument
    // is construed as the step length.
    Ray(const Point &p, const Direction &d);
    Ray();

    // Get ray position.
    const Point get_position() const;

    // Get ray origin.
    const Point &get_origin() const;

    // Get ray parameter.
    float get_parameter() const;

    // Set parameter of ray
    void set_parameter(const float _t);

    // Set direction of ray
    void set_direction(const Direction &d);

    // Set origin of ray
    void set_origin(const Point &p);

    // Get direction of ray.
    const Direction &get_direction() const;

    const Ray &operator=(const Ray &r);
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

Direction get_random_unit_vector();

float get_random_value(const float a, const float b);

#endif // !GEOMETRY_HPP