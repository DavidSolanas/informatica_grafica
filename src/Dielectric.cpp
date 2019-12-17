/****************************************+
 * Fichero: Dielectric.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Dielectric.hpp"
#include <cmath>
#include "Transformation.hpp"

Dielectric::Dielectric(const RGB &_ks, const RGB &_kt, const float ior) : BRDF(RGB(.0, .0, .0), RGB(.0, .0, .0), _ks, _kt), idx_of_refraction(ior) {}

RGB Dielectric::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const
{
    ro.set_origin(ri.get_position());
    ro.set_parameter(INFINITY);

    // Calcular probabilidades y normalizarlas
    float pkps;
    float pkpr;
    fresnel_law(n, ri.get_direction(), idx_of_refraction, pkps, pkpr);
    pkps *= .9;
    pkpr *= .9;
    float err = get_random_value(0.0f, 1.0f);

    if (err < pkps)
    {
        // Especular perfecto
        ro.set_direction(get_reflection(n, ri.get_direction()));
        //pdf = 1 / (2 * M_PI);
        return delta_BRDF(kps, ri, n, ro);
    }
    else if (err >= pkps && err < pkps + pkpr)
    {
        // Refracción perfecta
        Direction out_direction = get_refraction(n, ri.get_direction(), idx_of_refraction);
        // Total internal reflection
        if (out_direction.mod() == 0)
            out_direction = get_reflection(n * -1, ri.get_direction());
        ro.set_direction(out_direction);
        //pdf = 1 / (2 * M_PI);
        return delta_BTDF(kpr, ri, n, ro, idx_of_refraction);
    }
    else
    {
        //MATAR RAYO
        //pdf = -1;
        return RGB(-1, -1, -1);
    }
}

RGB Dielectric::get_difusse() const
{
    return kd;
}

RGB Dielectric::get_specular() const
{
    return ks;
}

RGB Dielectric::get_perfect_specular() const
{
    return kps;
}

RGB Dielectric::get_perfect_refractive() const
{
    return kpr;
}

RGB Dielectric::get_fr(const Ray &ri, const Direction &n, const Ray &ro) const
{
    return RGB(0, 0, 0);
}

bool Dielectric::is_delta() const
{
    return false;
}