/****************************************+
 * Fichero: BRDF.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef BRDF_HPP
#define BRDF_HPP

#include "Geometry.hpp"

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