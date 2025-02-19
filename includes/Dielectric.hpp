/****************************************+
 * Fichero: Dielectric.hpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "BRDF.hpp"

class Dielectric : public BRDF
{
private:
    float idx_of_refraction;

public:
    Dielectric(const RGB &_ks, const RGB &_kt, const float ior);
    ~Dielectric() {}
    RGB get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const override;
    RGB get_difusse() const override;
    RGB get_specular() const override;
    RGB get_perfect_specular() const override;
    RGB get_perfect_refractive() const override;
    RGB get_fr(const Ray &ri, const Direction &n, const Ray &ro) const override;
    bool is_delta() const override;
};

#endif // !Dielectric_HPP