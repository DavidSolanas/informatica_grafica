/****************************************+
 * Fichero: BRDF.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "BRDF.hpp"
#include <cmath>

Direction get_reflection(const Direction &n, const Direction &wi)
{
    return normalize(wi - n * (2 * dot(wi, n)));
}

Direction get_refraction(const Direction &n, const Direction &wi, const float ior1, const float ior2)
{
    Direction _n = n;
    float ior_in = ior1;
    float ior_out = ior2;
    float n_wi = dot(n, wi);
    // Comprobaciones para ver si el rayo va desde el exterior al interior o viceversa
    // y aplicar la ecuación correctamente
    if (n_wi < 0)
    {
        // Fuera de la superficie, cos(th1) tiene que ser positivo
        n_wi = -n_wi;
    }
    else
    {
        // Dentro de la superficie, cos(th1) positivo pero hay que cambiar n de sentido y los
        // indices de refracción
        _n = n * -1;
        float aux = ior_in;
        ior_in = ior_out;
        ior_out = aux;
    }

    float ior = ior_in / ior_out;
    float d = 1 - ior * ior * (1 - n_wi * n_wi);
    if (d < 0)
    {
        // No hay refracción --> total internal reflection
        return Direction(0, 0, 0);
    }
    return normalize((wi + _n * n_wi) * ior - _n * sqrt(d));
}

void fresnel_law(const Direction &n, const Direction &wi, const float ior1, const float ior2,
                 float &fr, float &ft)
{
    Direction _n = n;
    float ior_in = ior1;
    float ior_out = ior2;
    float n_wi = dot(n, wi);
    // Comprobaciones para ver si el rayo va desde el exterior al interior o viceversa
    // y aplicar la ecuación correctamente
    if (n_wi < 0)
    {
        // Fuera de la superficie, cos(th1) tiene que ser positivo
        n_wi = -n_wi;
    }
    else
    {
        // Dentro de la superficie, cos(th1) positivo pero hay que cambiar n de sentido y los
        // indices de refracción
        _n = n * -1;
        float aux = ior_in;
        ior_in = ior_out;
        ior_out = aux;
    }

    float ior = ior_in / ior_out;
    float sin_i = 1 - n_wi * n_wi < 0 ? 0 : sqrt(1 - n_wi * n_wi);
    float sin_o = ior * sin_i;
    if (sin_o >= 1)
    {
        // No hay refracción --> total internal reflection
        fr = 1; // Reflected coefficient
        ft = 0; // Refracted coefficient
    }
    else
    {
        float cos_o = 1 - sin_o * sin_o < 0 ? 0 : sqrt(1 - sin_o * sin_o);
        float rs = (ior_in * n_wi - ior_out * cos_o) / (ior_in * n_wi + ior_out * cos_o);
        float rp = (ior_in * cos_o - ior_out * n_wi) / (ior_in * cos_o + ior_out * n_wi);
        fr = (rs * rs + rp * rp) / 2;
        ft = 1 - fr;
    }
}

float phong_BRDF(const float kd, const float ks, const float alpha, Direction n,
                 Direction wi, Direction wo)
{
    Direction wr = get_reflection(n, wi);
    return (kd / M_PI) + ((ks * (alpha + 2) / (2 * M_PI)) * pow(abs(dot(wr, wo)), alpha));
}

float lambertian_BRDF(const float kd)
{
    return (kd / M_PI);
}