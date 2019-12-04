/****************************************+
 * Fichero: BRDF.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "BRDF.hpp"

class Lambertian : public BRDF
{
private:
    RGB kd;

public:
    Lambertian(const RGB &_kd);
    ~Lambertian() {}
    void get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const override;
    RGB get_albedo(const Ray &ray, const Direction &n, const Point &hit) const override;
    float get_specular(const Ray &ray, const Direction &n, const Point &hit) const override;
    bool is_delta() const override;
};

#endif // !LAMBERTIAN_HPP