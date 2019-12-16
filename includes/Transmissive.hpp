/****************************************+
 * Fichero: Transmissive.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef TRANSMISSIVE_HPP
#define TRANSMISSIVE_HPP

#include "BRDF.hpp"

class Transmissive : public BRDF
{
private:
    float idx_of_refraction;

public:
    Transmissive(const RGB &kpr, const float ior);
    ~Transmissive() {}
    RGB get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const override;
    RGB get_difusse() const override;
    RGB get_specular() const override;
    RGB get_perfect_specular() const override;
    RGB get_perfect_refractive() const override;
    RGB get_fr(const Ray &ri, const Direction &n, const Ray &ro) const override;
    bool is_delta() const override;
};

#endif // !TRANSMISSIVE_HPP