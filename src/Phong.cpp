/****************************************+
 * Fichero: Phong.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Phong.hpp"
#include <cmath>
#include <random>
#include "Transformation.hpp"

Phong::Phong(const RGB &_kd, const RGB &_ks, const float shiny) : BRDF(_kd, _ks, RGB(.0, .0, .0), RGB(.0, .0, .0)), shininess(shiny) {}

RGB Phong::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const
{
    ro.set_origin(ri.get_position());
    ro.set_parameter(INFINITY);

    // Calcular probabilidades y normalizarlas
    float pkd = kd.max();
    float pks = ks.max();
    float err = get_random_value(0.0f, 1.0f);
    if (err < pkd)
    {
        //Difuso
        ro.set_direction(get_cosine_ray(n, ro.get_origin()));

        return kd / pkd;
    }
    else if (err >= pkd && err < (pkd + pks))
    {
        //Especular (phong)
        ro.set_direction(get_cosine_ray(n, ro.get_origin()));

        Direction wi = normalize(ro.get_direction());
        Direction wo = normalize(ri.get_direction() * -1);
        Direction wr = get_reflection(n, wi);
        return (ks * (shininess + 2) * pow(fabs(dot(wr, wo)), shininess)) / 2;
    }
    else
    {
        //MATAR RAYO
        return RGB(-1, -1, -1);
    }
}

RGB Phong::get_difusse() const
{
    return kd;
}

RGB Phong::get_specular() const
{
    return ks;
}

RGB Phong::get_perfect_specular() const
{
    return kps;
}

RGB Phong::get_perfect_refractive() const
{
    return kpr;
}

RGB Phong::get_fr(const Ray &ri, const Direction &n, const Ray &ro) const
{
    Direction wi = ro.get_direction() * -1;
    Direction wo = ri.get_direction() * -1;
    Direction wr = get_reflection(n, wi);
    return (kd / M_PI) + ((ks * (shininess + 2) / (2 * M_PI)) * pow(fabs(dot(wr, wo)), shininess));
}

bool Phong::is_delta() const
{
    return false;
}