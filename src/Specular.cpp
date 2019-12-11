/****************************************+
 * Fichero: Specular.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Specular.hpp"
#include <cmath>
#include "Transformation.hpp"

Specular::Specular(const RGB &_kps) : BRDF(RGB(.0, .0, .0), RGB(.0, .0, .0), _kps, RGB(.0, .0, .0)) {}

void Specular::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const
{
    ro.set_origin(ri.get_position());
    //ro.set_direction(RandomUnitVectorInHemisphereOf(n, ro.get_origin()));
    ro.set_parameter(INFINITY);
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
}

bool Specular::is_delta() const
{
    return true;
}