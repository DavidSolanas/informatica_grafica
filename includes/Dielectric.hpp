/****************************************+
 * Fichero: Dielectric.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "BRDF.hpp"

class Dielectric : public BRDF
{
private:
public:
    Dielectric(const RGB &_ks, const RGB &_kt);
    ~Dielectric() {}
    void get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const override;
    RGB get_difusse() const override;
    RGB get_specular() const override;
    RGB get_perfect_specular() const override;
    RGB get_perfect_refractive() const override;
    bool is_delta() const override;
};

#endif // !Dielectric_HPP