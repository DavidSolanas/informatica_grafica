
/****************************************+
 * Fichero: Sphere.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Geometry.hpp"

/**
 * Clase que representa un Spherea, un Spherea
 * tiene un centro (Punto), un eje (Dirección) y
 * una ciudad de referencia (Punto)
 */
class Sphere : public Geometry
{
private:
    /**
     * Centro del Spherea
     */
    Point center;
    /**
     * Eje del Spherea
     */
    Direction axis;
    /**
     * Ciudad de referencia
     */
    Point city;

public:
    Sphere();
    Sphere(const Point &center, const Direction &axis, const Point &city);
    Sphere &operator=(const Sphere &p);
    Point getCenter();
    Direction getAxis();
    Point getCity();
    float getRadius();
    const float getAzimuth() const;
    Direction getNormal(Point X) override;
    bool intersect(const Point &p, const Direction &D, float &t) override;
};

/**
 * Clase que representa una estación, una estación
 * tiene una longitud (rad), una latitud (rad) y un Spherea (Sphere).
 */
class SphereGeometry
{
private:
    /**
     * Inclinación de la estación
     */
    float inclination;
    /**
     * Azimuth de la estación
     */
    float azimuth;
    /**
     * Spherea de la estación
     */
    Sphere sphere;

public:
    /**
     * Constructor de la clase SphereGeometry
     */
    SphereGeometry(const float &inc, const float &az, const Sphere &p);
    /**
     * Devuelve la posición en el Spherea de la estación
     */
    Point getPosition();
    /**
     * Devuelve la normal a la superficie de la estación
     * (módulo 1), coordenada K
     */
    Direction getNormal();
    /**
     * Devuelve el vector tangente a la superficie del
     * Spherea y perpendicular al eje del Spherea (mod 1).
     * Coordenada I
     */
    Direction getLongitudeTD();
    /**
     * Devuelve el vector tangente a la superficie del
     * Spherea y perpendicular al otro vector tangente (Longitude),
     * (mod 1). Coordenada J
     */
    Direction getLatitudeTD();
};

/**
 * Devuelve un punto en la superficie del Spherea
 * según su azimuth e inclination
 */
Point getSurfacePoint(Sphere p, const float az, const float inc);

#endif // !SPHERE_HPP
