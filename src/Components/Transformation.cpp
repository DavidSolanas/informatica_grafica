/****************************************+
 * Fichero: Transformation.cpp
 * Autor: David Solanas
 *****************************************/

#include "Transformation.hpp"
#include <cmath>

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

    M[3][3] = 1;
}

const Direction Matrix_Transformation::operator*(const Direction &d) const
{
    std::array<float, 4> res = {0, 0, 0, 0};
    auto cd = d.getCoord();
    for (int i = 0; i < 4; i++)
    {
        // fila i-esima de la matriz
        float aux[] = {M[i][0], M[i][1], M[i][2], M[i][3]};
        res[i] = aux[0] * cd[0] + aux[1] * cd[1] + aux[2] * cd[2] + aux[3] * cd[3];
    }
    Direction dr;
    dr.setCoord(res);
    return dr;
}

const Point Matrix_Transformation::operator*(const Point &p) const
{
    std::array<float, 4> res = {0, 0, 0, 0};
    auto cp = p.getCoord();
    for (int i = 0; i < 4; i++)
    {
        // fila i-esima de la matriz
        float aux[] = {M[i][0], M[i][1], M[i][2], M[i][3]};
        res[i] = aux[0] * cp[0] + aux[1] * cp[1] + aux[2] * cp[2] + aux[3] * cp[3];
    }
    Point pr;
    pr.setCoord(res);
    return pr;
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