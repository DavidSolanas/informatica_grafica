/****************************************+
 * Fichero: Transformation.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Geometry.hpp"

class Matrix_Transformation
{
private:
    std::array<std::array<float, 4>, 4> M;

public:
    /**
     * Constructor para una matriz de traslacion (op = 0) o
     * de escalado (op = 1)
     */
    Matrix_Transformation(const float x, const float y, const float z, const int op);
    /**
     * Constructor para una matriz de rotación en el eje
     * X (axis = 0), Y (axis = 1), o Z (axis = 2)
     */
    Matrix_Transformation(const float theta, const int axis);
    /**
     * Constructor para una matriz de cambio de base
     */
    Matrix_Transformation(const Point &p, const Direction &u,
                          const Direction &v, const Direction &w);
    /**
     * Constructor para una matriz de cambio de base
     */
    Matrix_Transformation(const Direction &n);
    /**
     * Constructor para una matriz dados los valores de la matriz
     */
    Matrix_Transformation(std::array<std::array<float, 4>, 4> A);
    /**
     * Multiplica una matriz 4x4 por un vector dirección,
     * devuelve un vector dirección
     */
    const Direction operator*(const Direction &d) const;
    /**
     * Multiplica una matriz 4x4 por un punto,
     * devuelve un punto
     */
    const Point operator*(const Point &p) const;

    /**
     * Devuelve la matriz inversa de la matriz actual
     */
    Matrix_Transformation inverse();

    /**
     * Devuelve la matriz
     */
    std::array<std::array<float, 4>, 4> getMatrix();
};

Point translation(const Point &p, const float x,
                  const float y, const float z);

Direction translation(const Direction &d, const float x,
                      const float y, const float z);

Point scale(const Point &p, const float x,
            const float y, const float z);

Direction scale(const Direction &d, const float x,
                const float y, const float z);

Point rotate(const Point &p, const float theta, const int axis);

Direction rotate(const Direction &d, const float theta, const int axis);

Point change_base(const Point &p, const Point &origin, const Direction &u,
                  const Direction &v, const Direction &w);

Direction change_base(const Direction &d, const Point &origin, const Direction &u,
                      const Direction &v, const Direction &w);

#endif // !TRANSFORM_HPP