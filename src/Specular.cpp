/****************************************+
 * Fichero: Specular.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Specular.hpp"
#include <cmath>
#include "Transformation.hpp"

Specular::Specular(const RGB &_kps) : BRDF(RGB(.0, .0, .0), RGB(.0, .0, .0), _kps, RGB(.0, .0, .0)) {}

RGB Specular::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const
{
    ro.set_origin(ri.get_position());
    ro.set_parameter(INFINITY);

    // Calcular probabilidades y normalizarlas
    float pkps = kps.max();
    float err = get_random_value(0.0f, 1.0f);

    if (err < pkps)
    {
        // Especular perfecto
        ro.set_direction(get_reflection(n, ri.get_direction()));
        return kps / pkps;
    }
    else
    {
        //MATAR RAYO
        return RGB(-1, -1, -1);
    }
}

RGB Specular::get_difusse() const
{
    return kd;
}

RGB Specular::get_specular() const
{
    return ks;
}

RGB Specular::get_perfect_specular() const
{
    return kps;
}

RGB Specular::get_perfect_refractive() const
{
    return kpr;
}

RGB Specular::get_fr(const Ray &ri, const Direction &n, const Ray &ro) const
{
    return kps / fabs(dot(ro.get_direction(), n));
}

bool Specular::is_delta() const
{
    return true;
}