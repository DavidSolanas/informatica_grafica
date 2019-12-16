/****************************************+
 * Fichero: Material.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Material.hpp"
#include <cmath>
#include "Transformation.hpp"

Material::Material(const RGB &_kd, const RGB &_ks, const RGB &_kps, const RGB &_kpr, const float sh, const float ior) : BRDF(_kd, _ks, _kps, _kpr),
                                                                                                                        shininess(sh), idx_of_refraction(ior) {}

RGB Material::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const
{
    ro.set_origin(ri.get_position());
    ro.set_parameter(INFINITY);

    // Calcular probabilidades y normalizarlas
    float pkd = kd.max() * .9;
    float pks = ks.max() * .9;
    float pkps = kps.max() * .9;
    float pkpr = kpr.max() * .9;
    float err = get_random_value(0.0f, 1.0f);
    if (err < pkd)
    {
        //Difuso
        ro.set_direction(get_cosine_ray(n, ro.get_origin()));
        //pdf = fabs(dot(ro.get_direction(), n)) / M_PI;
        return lambertian_BRDF(kd);
    }
    else if (err >= pkd && err < pkd + pks)
    {
        //Especular (phong)
        ro.set_direction(get_cosine_ray(n, ro.get_origin()));
        //pdf = fabs(dot(ro.get_direction(), n)) / M_PI;
        return phong_specular_BRDF(ks, shininess, ri, n, ro);
    }
    else if (err >= pkd + pks && err < pkd + pks + pkps)
    {
        // Especular perfecto
        ro.set_direction(get_reflection(n, ri.get_direction()));
        //pdf = 1 / (2 * M_PI);
        return delta_BRDF(kps, ri, n, ro);
    }
    else if (err >= pkd + pks + pkps && err < pkd + pks + pkps + pkpr)
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

RGB Material::get_difusse() const
{
    return kd;
}

RGB Material::get_specular() const
{
    return ks;
}

RGB Material::get_perfect_specular() const
{
    return kps;
}

RGB Material::get_perfect_refractive() const
{
    return kpr;
}

RGB Material::get_fr(const Ray &ri, const Direction &n, const Ray &ro) const
{
}

bool Material::is_delta() const
{
    return false;
}