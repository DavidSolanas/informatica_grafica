/****************************************+
 * Fichero: BRDF.cpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#include "BRDF.hpp"
#include <cmath>
#include <random>
#include "Transformation.hpp"
#include <iostream>

Direction get_reflection(const Direction &n, const Direction &wi)
{
    return normalize(wi - (n * dot(wi, n) * 2));
}

Direction get_refraction(const Direction &n, const Direction &wi, const float idx_r)
{
    Direction _n = n;
    float ior_in = AIR_REFRACTION_INDEX;
    float ior_out = idx_r;
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
    return normalize(wi * ior + _n * (ior * n_wi - sqrt(d)));
}

void fresnel_law(const Direction &n, const Direction &wi, const float idx_r,
                 float &fr, float &ft)
{
    Direction _n = n;
    float ior_in = AIR_REFRACTION_INDEX;
    float ior_out = idx_r;
    float n_wi = dot(n, wi);
    //Clamp n_wi -- cos(th1)
    n_wi = n_wi < -1 ? -1.f : n_wi;
    n_wi = n_wi > 1 ? 1.f : n_wi;
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

Direction get_cosine_ray(const Direction &n, const Point &x)
{
    float ethi = get_random_value(0.0f, 1.0f);
    float ephi = get_random_value(0.0f, 1.0f);
    //Cambio de coordenadas locales
    Matrix_Transformation T(n, x);
    float thi = acosf(sqrt(1 - ethi));
    float phi = 2 * M_PI * ephi;
    float _x = sin(thi) * cos(phi);
    float _y = cos(thi);
    float _z = sin(thi) * sin(phi);
    Direction wi(_x, _y, _z);
    return normalize(T * normalize(wi));
}

RGB delta_BRDF(const RGB &kps, const Ray &ri, const Direction &n, const Ray &ro)
{
    return kps / fabs(dot(ro.get_direction(), n));
}

RGB delta_BTDF(const RGB &kpr, const Ray &ri, const Direction &n, const Ray &ro, const float idx_refraction)
{
    float ior_in = AIR_REFRACTION_INDEX;
    float ior_out = idx_refraction;
    float n_wi = dot(n, ri.get_direction());
    // Comprobaciones para ver si el rayo va desde el exterior al interior o viceversa
    // y aplicar la ecuación correctamente
    if (n_wi >= 0)
    {
        // hay que cambiar n de sentido y los
        // indices de refracción
        float aux = ior_in;
        ior_in = ior_out;
        ior_out = aux;
    }
    float ior = ior_in / ior_out;
    return kpr * ior * ior / fabs(dot(ro.get_direction(), n));
}

RGB phong_specular_BRDF(const RGB &ks, const float alpha, const Ray &ri, const Direction &n, const Ray &ro)
{
    Direction wi = ro.get_direction() * -1;
    Direction wo = ri.get_direction() * -1;
    Direction wr = get_reflection(n, wi);
    return (ks * (alpha + 2) / (2 * M_PI)) * pow(fabs(dot(wr, wo)), alpha);
}

RGB lambertian_BRDF(const RGB &kd)
{
    return (kd / M_PI);
}