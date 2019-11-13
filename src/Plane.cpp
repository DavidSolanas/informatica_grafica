/****************************************+
 * Fichero: Plane.cpp
 * Autor: David Solanas
 *****************************************/

#include "Plane.hpp"
#include <iostream>

Plane::Plane() {}

Plane::Plane(const Direction &n, const Point &o)
{
    Direction n_normalized = normalize(n);
    std::array<float, 4> cn = n_normalized.getCoord();
    this->a = cn[0];
    this->b = cn[1];
    this->c = cn[2];
    this->d = -dot(n_normalized, o);
}

Plane::Plane(const Point &a, const Point &b, const Point &c)
{
    Direction n = normalize(cross(b - a, c - b));
    std::array<float, 4> cn = n.getCoord();
    this->a = cn[0];
    this->b = cn[1];
    this->c = cn[2];
    this->d = -dot(n, a);
}

float Plane::getD()
{
    return this->d;
}

Direction Plane::getNormal()
{
    return Direction(this->a, this->b, this->c);
}

Direction Plane::getNormal(Point X)
{
    return this->getNormal();
}

bool Plane::isInPlane(const Point &p)
{
    return dot(this->getNormal(), p) + this->d == 0;
}

bool Plane::intersect(const Point &p, const Direction &D, float &t)
{
    t = -(dot(this->getNormal(), p) + this->d) / (dot(D, this->getNormal()));
    if (t <= 0)
        return false;
    return true;
}

BoundedPlane::BoundedPlane(const Point &_A, const Point &_B,
                           const Point &_C, const Point &_D) : Plane(_A, _B, _C)
{
    this->A = _A;
    this->B = _B;
    this->C = _C;
    this->D = _D;
}

/**
 * D = (x2 - x1) * (yp - y1) - (xp - x1) * (y2 - y1)
 */
bool BoundedPlane::isInsidePlane(const Point &p)
{
    char dim = 'x';
    if (this->b > this->a && this->b > this->c)
        dim = 'y';
    else if (this->c > this->a && this->c > this->b)
        dim = 'z';
    int m[2][3];
    switch (dim)
    {
        /*  
        Reducir dimensión X
            | 0  1  0 |
            | 0  0  1 |
        */
    case 'x':
        m[0][0] = 0;
        m[0][1] = 1;
        m[0][2] = 0;
        m[1][0] = 0;
        m[1][1] = 0;
        m[1][2] = 1;
        break;
        /*  
        Reducir dimensión Y
            | 1  0  0 |
            | 0  0  1 |
        */
    case 'y':
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[1][0] = 0;
        m[1][1] = 0;
        m[1][2] = 1;
        break;
        /*  
        Reducir dimensión Z
            | 1  0  0 |
            | 0  1  0 |
        */
    case 'z':
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        break;

    default:
        break;
    }
    // Project the points to the plane (to the one with the largerst magnitude in the normal)
    std::array<float, 4> ac = A.getCoord();
    std::array<float, 4> bc = B.getCoord();
    std::array<float, 4> cc = C.getCoord();
    std::array<float, 4> dc = D.getCoord();
    std::array<float, 4> pc = p.getCoord();
    float _A[2] = {m[0][0] * ac[0] + m[0][1] * ac[1] + m[0][2] * ac[2],
                   m[1][0] * ac[0] + m[1][1] * ac[1] + m[1][2] * ac[2]};
    float _B[2] = {m[0][0] * bc[0] + m[0][1] * bc[1] + m[0][2] * bc[2],
                   m[1][0] * bc[0] + m[1][1] * bc[1] + m[1][2] * bc[2]};
    float _C[2] = {m[0][0] * cc[0] + m[0][1] * cc[1] + m[0][2] * cc[2],
                   m[1][0] * cc[0] + m[1][1] * cc[1] + m[1][2] * cc[2]};
    float _D[2] = {m[0][0] * dc[0] + m[0][1] * dc[1] + m[0][2] * dc[2],
                   m[1][0] * dc[0] + m[1][1] * dc[1] + m[1][2] * dc[2]};
    float _P[2] = {m[0][0] * pc[0] + m[0][1] * pc[1] + m[0][2] * pc[2],
                   m[1][0] * pc[0] + m[1][1] * pc[1] + m[1][2] * pc[2]};
    //float s1 = (_A[1] - _B[1]) * _P[0] + (_B[0] - _A[0]) * _P[1] + _B[1] * _A[0] - _B[0] * _A[1];
    //float s2 = (_B[1] - _C[1]) * _P[0] + (_C[0] - _B[0]) * _P[1] + _C[1] * _B[0] - _C[0] * _B[1];
    //float s3 = (_C[1] - _D[1]) * _P[0] + (_D[0] - _C[0]) * _P[1] + _D[1] * _C[0] - _D[0] * _C[1];
    //float s4 = (_D[1] - _A[1]) * _P[0] + (_A[0] - _D[0]) * _P[1] + _A[1] * _D[0] - _A[0] * _D[1];

    float s1 = (_B[0] - _A[0]) * (_P[1] - _A[1]) - (_P[0] - _A[0]) * (_B[1] - _A[1]);
    float s2 = (_C[0] - _B[0]) * (_P[1] - _B[1]) - (_P[0] - _B[0]) * (_C[1] - _B[1]);
    float s3 = (_D[0] - _C[0]) * (_P[1] - _C[1]) - (_P[0] - _C[0]) * (_D[1] - _C[1]);
    float s4 = (_A[0] - _D[0]) * (_P[1] - _D[1]) - (_P[0] - _D[0]) * (_A[1] - _D[1]);

    if ((s1 <= 0 && s2 <= 0 && s3 <= 0 && s4 <= 0) ||
        (s1 >= 0 && s2 >= 0 && s3 >= 0 && s4 >= 0))
    {
        return true;
    }
    return false;
}

bool BoundedPlane::intersect(const Point &p, const Direction &D, float &t)
{
    t = -(dot(this->getNormal(), p) + this->d) / (dot(D, this->getNormal()));
    if (t <= 0)
        return false;
    return isInsidePlane(p + (D * t));
}