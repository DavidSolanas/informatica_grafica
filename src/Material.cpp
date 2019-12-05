/****************************************+
 * Fichero: Material.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Material.hpp"
#include <cmath>
#include "Transformation.hpp"

Material::Material(const RGB &_kd, const RGB &_ks, const RGB &_kps, const RGB &_kpr) : BRDF(_kd, _ks, _kps, _kpr) {}

void Material::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const
{
    ro.set_origin(ri.get_position());
    //ro.set_direction(RandomUnitVectorInHemisphereOf(n, ro.get_origin()));
    ro.set_parameter(INFINITY);
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