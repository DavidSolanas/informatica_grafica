/****************************************+
 * Fichero: Specular.hpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#ifndef SPECULAR_HPP
#define SPECULAR_HPP

#include "BRDF.hpp"

class Specular : public BRDF
{
private:
public:
    Specular(const RGB &kps);
    ~Specular() {}
    RGB get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const override;
    RGB get_difusse() const override;
    RGB get_specular() const override;
    RGB get_perfect_specular() const override;
    RGB get_perfect_refractive() const override;
    RGB get_fr(const Ray &ri, const Direction &n, const Ray &ro) const override;
    bool is_delta() const override;
};

#endif // !Specular_HPP