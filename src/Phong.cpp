/****************************************+
 * Fichero: Phong.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Phong.hpp"
#include <cmath>
#include <random>
#include "Transformation.hpp"

Phong::Phong(const RGB &_kd, const RGB &_ks, const float shiny) : BRDF(_kd, _ks, RGB(.0, .0, .0), RGB(.0, .0, .0)), shininess(shiny) {}

Direction RandomUnitVectorInHemisphereOf2(const Direction &n, const Point &p)
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

void Phong::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const
{
    ro.set_origin(ri.get_position());
    ro.set_direction(RandomUnitVectorInHemisphereOf2(n, ro.get_origin()));
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

RGB Phong::get_fr(const Ray &ri, const Direction &n, const Ray &ro) const
{
    Direction wi = ro.get_direction() * -1;
    Direction wo = ri.get_direction() * -1;
    Direction wr = get_reflection(n, wi);
    return (kd / M_PI) + ((ks * (shininess + 2) / (2 * M_PI)) * pow(fabs(dot(wr, wo)), shininess));
}

bool Phong::is_delta() const
{
    return false;
}