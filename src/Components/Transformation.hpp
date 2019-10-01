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
    float M[4][4];

public:
    Matrix_Transformation(const float x, const float y, const float z, const int op);
    Matrix_Transformation(const float theta, const int axis);
    Matrix_Transformation(const Point &p, const Direction &u,
                          const Direction &v, const Direction &w);
    const Direction operator*(const Direction &d) const;
    const Point operator*(const Point &d) const;
};

Matrix_Transformation::Matrix_Transformation(const float x, const float y,
                                             const float z, const int op)
{
    switch (op)
    {
    case 0: //Matriz de traslación
        M[0][0] = 1;
        M[1][1] = 1;
        M[2][2] = 1;
        M[3][3] = 1;
        M[0][3] = x;
        M[1][3] = y;
        M[2][3] = z;
        break;
    case 1: //Matriz de escalado
        M[0][0] = x;
        M[1][1] = y;
        M[2][2] = z;
        M[3][3] = 1;
        break;
    default:
        break;
    }
}
Matrix_Transformation::Matrix_Transformation(const float theta, const int axis)
{
}
Matrix_Transformation::Matrix_Transformation(const Point &p, const Direction &u,
                                             const Direction &v, const Direction &w)
{
}
const Direction Matrix_Transformation::operator*(const Direction &d) const
{
}
const Point Matrix_Transformation::operator*(const Point &d) const
{
}

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

Direction change_base(const Direction &p, const Point &origin, const Direction &u,
                      const Direction &v, const Direction &w);

#endif // !TRANSFORM_HPP