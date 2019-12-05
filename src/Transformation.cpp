/****************************************+
 * Fichero: Transformation.cpp
 * Autor: David Solanas
 *****************************************/

#include "Transformation.hpp"
#include <cmath>

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
    M[0][0] = u.x;
    M[1][0] = u.y;
    M[2][0] = u.z;

    M[0][1] = v.x;
    M[1][1] = v.y;
    M[2][1] = v.z;

    M[0][2] = w.x;
    M[1][2] = w.y;
    M[2][2] = w.z;

    M[0][3] = p.x;
    M[1][3] = p.y;
    M[2][3] = p.z;

    M[3][0] = 0;
    M[3][1] = 0;
    M[3][2] = 0;
    M[3][3] = 1;
}

Matrix_Transformation::Matrix_Transformation(const Direction &n, const Point &p)
{
    Direction dr = get_random_unit_vector();
    while (dot(dr, n) == n.mod() * dr.mod())
    {
        dr = get_random_unit_vector();
    }
    Direction x = cross(n, dr);
    Direction y = cross(n, x);

    M[0][0] = x.x;
    M[1][0] = x.y;
    M[2][0] = x.z;

    M[0][1] = y.x;
    M[1][1] = y.y;
    M[2][1] = y.z;

    M[0][2] = n.x;
    M[1][2] = n.y;
    M[2][2] = n.z;

    M[0][3] = p.x;
    M[1][3] = p.y;
    M[2][3] = p.z;

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
    //Cálculo del resultado
    float c1 = M[0][0] * d.x + M[0][1] * d.y + M[0][2] * d.z + M[0][3] * d.d;
    float c2 = M[1][0] * d.x + M[1][1] * d.y + M[1][2] * d.z + M[1][3] * d.d;
    float c3 = M[2][0] * d.x + M[2][1] * d.y + M[2][2] * d.z + M[2][3] * d.d;
    float c4 = M[3][0] * d.x + M[3][1] * d.y + M[3][2] * d.z + M[3][3] * d.d;

    Direction dr;
    dr.setCoord(c1, c2, c3, c4);
    return dr;
}

const Point Matrix_Transformation::operator*(const Point &p) const
{
    //Cálculo del resultado
    float c1 = M[0][0] * p.x + M[0][1] * p.y + M[0][2] * p.z + M[0][3] * p.d;
    float c2 = M[1][0] * p.x + M[1][1] * p.y + M[1][2] * p.z + M[1][3] * p.d;
    float c3 = M[2][0] * p.x + M[2][1] * p.y + M[2][2] * p.z + M[2][3] * p.d;
    float c4 = M[3][0] * p.x + M[3][1] * p.y + M[3][2] * p.z + M[3][3] * p.d;

    Point pr;
    pr.setCoord(c1, c2, c3, c4);
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
    return (A[0][0] * A[1][1] * A[2][2] * A[3][3] +
            A[0][0] * A[1][2] * A[2][3] * A[3][1] +
            A[0][0] * A[1][3] * A[2][1] * A[3][2] -
            A[0][0] * A[1][3] * A[2][2] * A[3][1] -
            A[0][0] * A[1][2] * A[2][1] * A[3][3] -
            A[0][0] * A[1][1] * A[2][3] * A[3][2]) -
           (A[1][0] * A[0][1] * A[2][2] * A[3][3] +
            A[1][0] * A[0][2] * A[2][3] * A[3][1] +
            A[1][0] * A[0][3] * A[2][1] * A[3][2] -
            A[1][0] * A[0][3] * A[2][2] * A[3][1] -
            A[1][0] * A[0][2] * A[2][1] * A[3][3] -
            A[1][0] * A[0][1] * A[2][3] * A[3][2]) +
           (A[2][0] * A[0][1] * A[1][2] * A[3][3] +
            A[2][0] * A[0][2] * A[1][3] * A[3][1] +
            A[2][0] * A[0][3] * A[1][1] * A[3][2] -
            A[2][0] * A[0][3] * A[1][2] * A[3][1] -
            A[2][0] * A[0][2] * A[1][1] * A[3][3] -
            A[2][0] * A[0][1] * A[1][3] * A[3][2]) -
           (A[3][0] * A[0][1] * A[1][2] * A[2][3] +
            A[3][0] * A[0][2] * A[1][3] * A[2][1] +
            A[3][0] * A[0][3] * A[1][1] * A[2][2] -
            A[3][0] * A[0][3] * A[1][2] * A[2][1] -
            A[3][0] * A[0][2] * A[1][1] * A[2][3] -
            A[3][0] * A[0][1] * A[1][3] * A[2][2]);
}

std::array<std::array<float, 4>, 4> adjugate(std::array<std::array<float, 4>, 4> A)
{
    std::array<std::array<float, 4>, 4> adj;
    adj[0][0] = A[1][1] * A[2][2] * A[3][3] +
                A[1][2] * A[2][3] * A[3][1] +
                A[1][3] * A[2][1] * A[3][2] -
                A[1][3] * A[2][2] * A[3][1] -
                A[1][2] * A[2][1] * A[3][3] -
                A[1][1] * A[2][3] * A[3][2];

    adj[0][1] = -A[0][1] * A[2][2] * A[3][3] -
                A[0][2] * A[2][3] * A[3][1] -
                A[0][3] * A[2][1] * A[3][2] +
                A[0][3] * A[2][2] * A[3][1] +
                A[0][2] * A[2][1] * A[3][3] +
                A[0][1] * A[2][3] * A[3][2];

    adj[0][2] = A[0][1] * A[1][2] * A[3][3] +
                A[0][2] * A[1][3] * A[3][1] +
                A[0][3] * A[1][1] * A[3][2] -
                A[0][3] * A[1][2] * A[3][1] -
                A[0][2] * A[1][1] * A[3][3] -
                A[0][1] * A[1][3] * A[3][2];

    adj[0][3] = -A[0][1] * A[1][2] * A[2][3] -
                A[0][2] * A[1][3] * A[2][1] -
                A[0][3] * A[1][1] * A[2][2] +
                A[0][3] * A[1][2] * A[2][1] +
                A[0][2] * A[1][1] * A[2][3] +
                A[0][1] * A[1][3] * A[2][2];

    adj[1][0] = -A[1][0] * A[2][2] * A[3][3] -
                A[1][2] * A[2][3] * A[3][0] -
                A[1][3] * A[2][0] * A[3][2] +
                A[1][3] * A[2][2] * A[3][0] +
                A[1][2] * A[2][0] * A[3][3] +
                A[1][0] * A[2][3] * A[3][2];

    adj[1][1] = A[0][0] * A[2][2] * A[3][3] +
                A[0][2] * A[2][3] * A[3][0] +
                A[0][3] * A[2][0] * A[3][2] -
                A[0][3] * A[2][2] * A[3][0] -
                A[0][2] * A[2][0] * A[3][3] -
                A[0][0] * A[2][3] * A[3][2];

    adj[1][2] = -A[0][0] * A[1][2] * A[3][3] -
                A[0][2] * A[1][3] * A[3][0] -
                A[0][3] * A[1][0] * A[3][2] +
                A[0][3] * A[1][2] * A[3][0] +
                A[0][2] * A[1][0] * A[3][3] +
                A[0][0] * A[1][3] * A[3][2];

    adj[1][3] = A[0][0] * A[1][2] * A[2][3] +
                A[0][2] * A[1][3] * A[2][0] +
                A[0][3] * A[1][0] * A[2][2] -
                A[0][3] * A[1][2] * A[2][0] -
                A[0][2] * A[1][0] * A[2][3] -
                A[0][0] * A[1][3] * A[2][2];

    adj[2][0] = A[1][0] * A[2][1] * A[3][3] +
                A[1][1] * A[2][3] * A[3][0] +
                A[1][3] * A[2][0] * A[3][1] -
                A[1][3] * A[2][1] * A[3][0] -
                A[1][1] * A[2][0] * A[3][3] -
                A[1][0] * A[2][3] * A[3][1];

    adj[2][1] = -A[0][0] * A[2][1] * A[3][3] -
                A[0][1] * A[2][3] * A[3][0] -
                A[0][3] * A[2][0] * A[3][1] +
                A[0][3] * A[2][1] * A[3][0] +
                A[0][1] * A[2][0] * A[3][3] +
                A[0][0] * A[2][3] * A[3][1];

    adj[2][2] = A[0][0] * A[1][1] * A[3][3] +
                A[0][1] * A[1][3] * A[3][0] +
                A[0][3] * A[1][0] * A[3][1] -
                A[0][3] * A[1][1] * A[3][0] -
                A[0][1] * A[1][0] * A[3][3] -
                A[0][0] * A[1][3] * A[3][1];

    adj[2][3] = -A[0][0] * A[1][1] * A[2][3] -
                A[0][1] * A[1][3] * A[2][0] -
                A[0][3] * A[1][0] * A[2][1] +
                A[0][3] * A[1][1] * A[2][0] +
                A[0][1] * A[1][0] * A[2][3] +
                A[0][0] * A[1][3] * A[2][1];

    adj[3][0] = -A[1][0] * A[2][1] * A[3][2] -
                A[1][1] * A[2][2] * A[3][0] -
                A[1][2] * A[2][0] * A[3][1] +
                A[1][2] * A[2][1] * A[3][0] +
                A[1][1] * A[2][0] * A[3][2] +
                A[1][0] * A[2][2] * A[3][1];

    adj[3][1] = A[0][0] * A[2][1] * A[3][2] +
                A[0][1] * A[2][2] * A[3][0] +
                A[0][2] * A[2][0] * A[3][1] -
                A[0][2] * A[2][1] * A[3][0] -
                A[0][1] * A[2][0] * A[3][2] -
                A[0][0] * A[2][2] * A[3][1];

    adj[3][2] = -A[0][0] * A[1][1] * A[3][2] -
                A[0][1] * A[1][2] * A[3][0] -
                A[0][2] * A[1][0] * A[3][1] +
                A[0][2] * A[1][1] * A[3][0] +
                A[0][1] * A[1][0] * A[3][2] +
                A[0][0] * A[1][2] * A[3][1];

    adj[3][3] = A[0][0] * A[1][1] * A[2][2] +
                A[0][1] * A[1][2] * A[2][0] +
                A[0][2] * A[1][0] * A[2][1] -
                A[0][2] * A[1][1] * A[2][0] -
                A[0][1] * A[1][0] * A[2][2] -
                A[0][0] * A[1][2] * A[2][1];

    return adj;
}

Matrix_Transformation Matrix_Transformation::inverse()
{
    float det = determinant(this->M);
    std::array<std::array<float, 4>, 4> adj = adjugate(this->M);
    std::array<std::array<float, 4>, 4> inv;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            inv[i][j] = 1.0 / det * adj[i][j];
        }
    }
    return Matrix_Transformation(inv);
}
