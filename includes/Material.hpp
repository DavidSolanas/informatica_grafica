/****************************************+
 * Fichero: Material.hpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "BRDF.hpp"

class Material : public BRDF
{
private:
    const float shininess;
    float idx_of_refraction;

public:
    Material(const RGB &_kd, const RGB &_ks, const RGB &_kps, const RGB &_kpr, const float sh, const float ior);
    ~Material() {}
    RGB get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const override;
    RGB get_difusse() const override;
    RGB get_specular() const override;
    RGB get_perfect_specular() const override;
    RGB get_perfect_refractive() const override;
    RGB get_fr(const Ray &ri, const Direction &n, const Ray &ro) const override;
    bool is_delta() const override;
};

#endif // !Material_HPP