/****************************************+
 * Fichero: Phong.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef PHONG_HPP
#define PHONG_HPP

#include "BRDF.hpp"

class Phong : public BRDF
{
private:
    const float shininess;

public:
    Phong(const RGB &_kd, const RGB &_ks, const float shiny);
    ~Phong() {}
    void get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const override;
    RGB get_difusse() const override;
    RGB get_specular() const override;
    RGB get_perfect_specular() const override;
    RGB get_perfect_refractive() const override;
    RGB get_fr(const Ray &ri, const Direction &n, const Ray &ro) const override;
    bool is_delta() const override;
};

#endif // !PHONG_HPP