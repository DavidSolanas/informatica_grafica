/****************************************+
 * Fichero: Lambertian.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Lambertian.hpp"
#include <random>
#include "Transformation.hpp"

Lambertian::Lambertian(const RGB &_kd) : BRDF(_kd, RGB(.0, .0, .0), RGB(.0, .0, .0), RGB(.0, .0, .0)) {}

Direction RandomUnitVectorInHemisphereOf(const Direction &n, const Point &p)
{
    //Cambio de coordenadas locales
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-1.f, 1.f);
    Matrix_Transformation T(n, p);
    float x = dist(mt), y = dist(mt), z = fabs(dist(mt));
    Direction _n(x, y, z);
    return normalize(T.inverse() * _n);
}

void Lambertian::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const
{
    ro.set_origin(ri.get_position());
    ro.set_direction(RandomUnitVectorInHemisphereOf(n, ro.get_origin()));
    ro.set_parameter(INFINITY);
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