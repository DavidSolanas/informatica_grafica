/****************************************+
 * Fichero: Lambertian.hpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "BRDF.hpp"

class Lambertian : public BRDF
{
private:
public:
    Lambertian(const RGB &_kd);
    ~Lambertian() {}
    RGB get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const override;
    RGB get_difusse() const override;
    RGB get_specular() const override;
    RGB get_perfect_specular() const override;
    RGB get_perfect_refractive() const override;
    RGB get_fr(const Ray &ri, const Direction &n, const Ray &ro) const override;
    bool is_delta() const override;
};

#endif // !LAMBERTIAN_HPP