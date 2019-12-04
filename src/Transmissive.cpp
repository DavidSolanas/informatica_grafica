/****************************************+
 * Fichero: Transmissive.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Transmissive.hpp"
#include <cmath>
#include "Transformation.hpp"

Transmissive::Transmissive(const RGB &kpr, const float ior) : idx_of_refraction(ior),
                                                              BRDF(RGB(.0, .0, .0), RGB(.0, .0, .0), RGB(.0, .0, .0), kpr) {}

void Transmissive::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const
{
    ro.set_origin(ri.get_position());
    //ro.set_direction(RandomUnitVectorInHemisphereOf(n, ro.get_origin()));
    ro.set_parameter(INFINITY);
}

RGB Transmissive::get_difusse() const
{
    return kd;
}

RGB Transmissive::get_specular() const
{
    return ks;
}

RGB Transmissive::get_perfect_specular() const
{
    return kps;
}

RGB Transmissive::get_perfect_refractive() const
{
    return kpr;
}

bool Transmissive::is_delta() const
{
    return true;
}