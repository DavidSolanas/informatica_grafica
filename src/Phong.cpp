/****************************************+
 * Fichero: Phong.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Phong.hpp"
#include <cmath>
#include "Transformation.hpp"

Phong::Phong(const RGB &_kd, const RGB &_ks, const float shiny) : shininess(shiny),
                                                                  BRDF(_kd, _ks, RGB(.0, .0, .0), RGB(.0, .0, .0)) {}

void Phong::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const
{
    ro.set_origin(ri.get_position());
    //ro.set_direction(RandomUnitVectorInHemisphereOf(n, ro.get_origin()));
    ro.set_parameter(INFINITY);
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

bool Phong::is_delta() const
{
    return false;
}