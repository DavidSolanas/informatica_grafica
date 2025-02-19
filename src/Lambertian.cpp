/****************************************+
 * Fichero: Lambertian.cpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#include "Lambertian.hpp"
#include <random>
#include "Transformation.hpp"

Lambertian::Lambertian(const RGB &_kd) : BRDF(_kd, RGB(.0, .0, .0), RGB(.0, .0, .0), RGB(.0, .0, .0)) {}

RGB Lambertian::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const
{
    ro.set_origin(ri.get_position());
    ro.set_parameter(INFINITY);

    // Calcular probabilidades y normalizarlas
    float pkd = kd.max();
    float err = get_random_value(0.0f, 1.0f);
    if (err < pkd)
    {
        //Difuso
        ro.set_direction(get_cosine_ray(n, ro.get_origin()));
        return kd / pkd;
    }
    else
    {
        //MATAR RAYO
        return RGB(-1, -1, -1);
    }
}

RGB Lambertian::get_difusse() const
{
    return kd;
}

RGB Lambertian::get_specular() const
{
    return ks;
}

RGB Lambertian::get_perfect_specular() const
{
    return kps;
}

RGB Lambertian::get_perfect_refractive() const
{
    return kpr;
}

RGB Lambertian::get_fr(const Ray &ri, const Direction &n, const Ray &ro) const
{
    return kd / M_PI;
}

bool Lambertian::is_delta() const
{
    return false;
}