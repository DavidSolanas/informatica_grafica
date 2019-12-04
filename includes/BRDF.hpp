

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

#include "RGB.hpp"

class BRDF
{
private:
public:
    BRDF() {}
    // Stochastically samples a new ray 'r' given a previous intersection 'it'. The 'pdf'
    // value is needed to get statistically correct results, but it is not needed for
    // specular and transparent objects (a parameter is needed, but it can just be ignored.
    virtual void get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro, float &pdf) const = 0;

    // Returns the albedo of the surface at intersection 'it'
    virtual RGB get_albedo(const Ray &ray, const Direction &n, const Point &hit) const = 0;
    // Returns the specular coefficient at intersection 'it'
    virtual float get_specular(const Ray &ray, const Direction &n, const Point &hit) const = 0;

    virtual bool is_delta() const = 0;
};

void fresnel_law(const Direction &n, const Direction &wi, const float ior1, const float ior2,
                 float &fr, float &ft);

Direction get_reflection(const Direction &n, const Direction &wi);

Direction get_refraction(const Direction &n, const Direction &wi, const float ior1, const float ior2);

// Perfect specular
float delta_BRDF(const Direction &n, const Direction &wi, const Direction &wo);

// Perfect refraction
float delta_BTDF(const Direction &n, const Direction &wi, const Direction &wo);

float phong_BRDF(const float kd, const float ks, const float alpha, const Direction &n,
                 const Direction &wi, const Direction &wo);

float lambertian_BRDF(const float kd);

#endif // !BRDF_HPP