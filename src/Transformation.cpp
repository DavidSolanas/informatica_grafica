/****************************************+
 * Fichero: Transformation.cpp
 * Autor: David Solanas
 *****************************************/

#include "Transformation.hpp"
#include <cmath>
#include <iostream>

Matrix_Transformation::Matrix_Transformation(const float x, const float y,
                                             const float z, const int op)
{
    //Inicialización de la matriz
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = 0;
        }
    }

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
    //Inicialización de la matriz
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = 0;
        }
    }

    switch (axis)
    {
    case 0: //X axis
        M[0][0] = 1;
        M[1][1] = cos(theta);
        M[2][2] = cos(theta);
        M[3][3] = 1;
        M[1][2] = -sin(theta);
        M[2][1] = sin(theta);
        break;
    case 1: //Y axis
        M[0][0] = cos(theta);
        M[1][1] = 1;
        M[2][2] = cos(theta);
        M[3][3] = 1;
        M[2][0] = -sin(theta);
        M[0][2] = sin(theta);
        break;
    case 2: //Z axis
        M[0][0] = cos(theta);
        M[1][1] = cos(theta);
        M[2][2] = 1;
        M[3][3] = 1;
        M[0][1] = -sin(theta);
        M[1][0] = sin(theta);
        break;
    default:
        break;
    }
}

Matrix_Transformation::Matrix_Transformation(const Point &p, const Direction &u,
                                             const Direction &v, const Direction &w)
{
    std::array<float, 4> cp = p.getCoord();
    std::array<float, 4> cu = u.getCoord();
    std::array<float, 4> cv = v.getCoord();
    std::array<float, 4> cw = w.getCoord();

    M[0][0] = cu[0];
    M[1][0] = cu[1];
    M[2][0] = cu[2];

    M[0][1] = cv[0];
    M[1][1] = cv[1];
    M[2][1] = cv[2];

    M[0][2] = cw[0];
    M[1][2] = cw[1];
    M[2][2] = cw[2];

    M[0][3] = cp[0];
    M[1][3] = cp[1];
    M[2][3] = cp[2];

    M[3][0] = 0;
    M[3][1] = 0;
    M[3][2] = 0;
    M[3][3] = 1;
}

/**
 * Constructor para una matriz dados los valores de la matriz
 */
Matrix_Transformation::Matrix_Transformation(std::array<std::array<float, 4>, 4> A)
{
    this->M = A;
}

const Direction Matrix_Transformation::operator*(const Direction &d) const
{
    std::array<float, 4> res = {0, 0, 0, 0};
    std::array<float, 4> cd = d.getCoord();
    //Cálculo del resultado
    res[0] = M[0][0] * cd[0] + M[0][1] * cd[1] + M[0][2] * cd[2] + M[0][3] * cd[3];
    res[1] = M[1][0] * cd[0] + M[1][1] * cd[1] + M[1][2] * cd[2] + M[1][3] * cd[3];
    res[2] = M[2][0] * cd[0] + M[2][1] * cd[1] + M[2][2] * cd[2] + M[2][3] * cd[3];
    res[3] = M[3][0] * cd[0] + M[3][1] * cd[1] + M[3][2] * cd[2] + M[3][3] * cd[3];

    Direction dr;
    dr.setCoord(res);
    return dr;
}

const Point Matrix_Transformation::operator*(const Point &p) const
{
    std::array<float, 4> res = {0, 0, 0, 0};
    std::array<float, 4> cp = p.getCoord();

    //Cálculo del resultado
    res[0] = M[0][0] * cp[0] + M[0][1] * cp[1] + M[0][2] * cp[2] + M[0][3] * cp[3];
    res[1] = M[1][0] * cp[0] + M[1][1] * cp[1] + M[1][2] * cp[2] + M[1][3] * cp[3];
    res[2] = M[2][0] * cp[0] + M[2][1] * cp[1] + M[2][2] * cp[2] + M[2][3] * cp[3];
    res[3] = M[3][0] * cp[0] + M[3][1] * cp[1] + M[3][2] * cp[2] + M[3][3] * cp[3];

    Point pr;
    pr.setCoord(res);
    return pr;
}

std::array<std::array<float, 4>, 4> Matrix_Transformation::getMatrix()
{
    return this->M;
}

Point translation(const Point &p, const float x,
                  const float y, const float z)
{
    Matrix_Transformation M(x, y, z, 0);
    return M * p;
}

Direction translation(const Direction &d, const float x,
                      const float y, const float z)
{
    Matrix_Transformation M(x, y, z, 0);
    return M * d;
}

Point scale(const Point &p, const float x,
            const float y, const float z)
{
    Matrix_Transformation M(x, y, z, 1);
    return M * p;
}

Direction scale(const Direction &d, const float x,
                const float y, const float z)
{
    Matrix_Transformation M(x, y, z, 1);
    return M * d;
}

Point rotate(const Point &p, const float theta, const int axis)
{
    Matrix_Transformation M(theta, axis);
    return M * p;
}

Direction rotate(const Direction &d, const float theta, const int axis)
{
    Matrix_Transformation M(theta, axis);
    return M * d;
}

Point change_base(const Point &p, const Point &origin, const Direction &u,
                  const Direction &v, const Direction &w)
{
    Matrix_Transformation M(origin, u, v, w);
    return M * p;
}

Direction change_base(const Direction &d, const Point &origin, const Direction &u,
                      const Direction &v, const Direction &w)
{
    Matrix_Transformation M(origin, u, v, w);
    return M * d;
}

float determinant(std::array<std::array<float, 4>, 4> A)
{
    return A[0][0] * (A[1][1] * A[2][2] * A[3][3] +
                      A[1][2] * A[2][3] * A[3][1] +
                      A[1][3] * A[2][1] * A[3][2] -
                      A[1][3] * A[2][2] * A[3][1] -
                      A[1][2] * A[2][1] * A[3][3] -
                      A[1][1] * A[2][3] * A[3][2]) -
           A[1][0] * (A[0][1] * A[2][2] * A[3][3] +
                      A[0][2] * A[2][3] * A[3][1] +
                      A[0][3] * A[2][1] * A[3][2] -
                      A[0][3] * A[2][2] * A[3][1] -
                      A[0][2] * A[2][1] * A[3][3] -
                      A[0][1] * A[2][3] * A[3][2]) +
           A[2][0] * (A[0][1] * A[1][2] * A[3][3] +
                      A[0][2] * A[1][3] * A[3][1] +
                      A[0][3] * A[1][1] * A[3][2] -
                      A[0][3] * A[1][2] * A[3][1] -
                      A[0][2] * A[1][1] * A[3][3] -
                      A[0][1] * A[1][3] * A[3][2]) -
           A[3][0] * (A[0][1] * A[1][2] * A[2][3] +
                      A[0][2] * A[1][3] * A[2][1] +
                      A[0][3] * A[1][1] * A[2][2] -
                      A[0][3] * A[1][2] * A[2][1] -
                      A[0][2] * A[1][1] * A[2][3] -
                      A[0][1] * A[1][3] * A[2][2]);
}

Matrix_Transformation Matrix_Transformation::inverse()
{
    //return Matrix_Transformation(inv);
    std::cout << determinant(this->M) << std::endl;
    return *this;
}