
/****************************************+
 * Fichero: Geometry.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef PLANETS_HPP
#define PLANETS_HPP

#include "Geometry.hpp"

/**
 * Clase que representa un planeta, un planeta
 * tiene un centro (Punto), un eje (Dirección) y
 * una ciudad de referencia (Punto)
 */
class Planet
{
private:
    /**
     * Centro del planeta
     */
    Point center;
    /**
     * Eje del planeta
     */
    Direction axis;
    /**
     * Ciudad de referencia
     */
    Point city;

public:
    Planet();
    Planet(const Point &center, const Direction &axis, const Point &city);
    Planet &operator=(const Planet &p);
    Point getCenter();
    Direction getAxis();
    Point getCity();
    float getRadius();
    const float getAzimuth() const;
};

/**
 * Clase que representa una estación, una estación
 * tiene una longitud (rad), una latitud (rad) y un planeta (Planet).
 */
class Station
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
     * Planeta de la estación
     */
    Planet planet;

public:
    /**
     * Constructor de la clase Station
     */
    Station(const float &inc, const float &az, const Planet &p);
    /**
     * Devuelve la posición en el planeta de la estación
     */
    Point getPosition();
    /**
     * Devuelve la normal a la superficie de la estación
     * (módulo 1), coordenada K
     */
    Direction getNormal();
    /**
     * Devuelve el vector tangente a la superficie del
     * planeta y perpendicular al eje del planeta (mod 1).
     * Coordenada I
     */
    Direction getLongitudeTD();
    /**
     * Devuelve el vector tangente a la superficie del
     * planeta y perpendicular al otro vector tangente (Longitude),
     * (mod 1). Coordenada J
     */
    Direction getLatitudeTD();
};

/**
 * Devuelve un punto en la superficie del planeta
 * según su azimuth e inclination
 */
Point getSurfacePoint(Planet p, const float az, const float inc);

#endif // !PLANETS_HPP
