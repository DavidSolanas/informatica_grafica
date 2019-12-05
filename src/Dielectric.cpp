/****************************************+
 * Fichero: Dielectric.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Dielectric.hpp"
#include <cmath>
#include "Transformation.hpp"

Dielectric::Dielectric(const RGB &_ks, const RGB &_kt) : BRDF(RGB(.0, .0, .0), RGB(.0, .0, .0), _ks, _kt) {}

void Dielectric::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const
{
    ro.set_origin(ri.get_position());
    //ro.set_direction(RandomUnitVectorInHemisphereOf(n, ro.get_origin()));
    ro.set_parameter(INFINITY);
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
}

bool Dielectric::is_delta() const
{
    return false;
}