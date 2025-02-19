/****************************************+
 * Fichero: BRDF.hpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#ifndef BRDF_HPP
#define BRDF_HPP

/// This is the refraction index of some materials / mediums.
const float AIR_REFRACTION_INDEX = 1.0f;
const float WATER_REFRACTION_INDEX = 1.333f;
const float GLASS_REFRACTION_INDEX = 1.52f;
const float SAPPHIRE_REFRACTION_INDEX = 1.77f;
const float DIAMOND_REFRACTION_INDEX = 2.42f;

class Point;
class Direction;
class Ray;

//lambertiano (coseno)
//phong (coseno)
//perfecto especular (ley de reflexion)
//perfecto refractor (ley de snell)
//dielectrico (ley de fresnell)

#include "RGB.hpp"

class BRDF
{
protected:
    // Difusse coefficient
    RGB kd;
    // Specular coefficient
    RGB ks;
    // Perfect specular coefficient
    RGB kps;
    // Perfect refractive coefficient
    RGB kpr;

public:
    BRDF() {}
    BRDF(const RGB &_kd, const RGB &_ks, const RGB &_kps, const RGB &_kpr) : kd(_kd), ks(_ks), kps(_kps), kpr(_kpr) {}
    // Stochastically samples a new ray 'r' given a previous intersection 'it'. The 'pdf'
    // value is needed to get statistically correct results, but it is not needed for
    // specular and transparent objects (a parameter is needed, but it can just be ignored.
    virtual RGB get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const = 0;

    // Returns the difusse coefficient
    virtual RGB get_difusse() const = 0;
    // Returns the specular coefficient
    virtual RGB get_specular() const = 0;
    // Returns the perfect specular coefficient
    virtual RGB get_perfect_specular() const = 0;
    // Returns the perfect refractive coefficient
    virtual RGB get_perfect_refractive() const = 0;
    // Returns the function fr for the point X and directions wi, wo
    virtual RGB get_fr(const Ray &ri, const Direction &n, const Ray &ro) const = 0;

    virtual bool is_delta() const = 0;
};

void fresnel_law(const Direction &n, const Direction &wi, const float idx_r,
                 float &fr, float &ft);

Direction get_reflection(const Direction &n, const Direction &wi);

Direction get_refraction(const Direction &n, const Direction &wi, const float idx_r);

Direction get_cosine_ray(const Direction &n, const Point &x);

// Perfect specular
RGB delta_BRDF(const RGB &kps, const Ray &ri, const Direction &n, const Ray &ro);

// Perfect refraction
RGB delta_BTDF(const RGB &kpr, const Ray &ri, const Direction &n, const Ray &ro, const float idx_refracion);

RGB phong_specular_BRDF(const RGB &ks, const float alpha, const Ray &ri, const Direction &n, const Ray &ro);

RGB lambertian_BRDF(const RGB &kd);

#endif // !BRDF_HPP