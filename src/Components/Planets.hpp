
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
    Station(const float &inc, const float &az, const Planet &p);
};

#endif // !PLANETS_HPP
