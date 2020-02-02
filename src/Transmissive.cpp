/****************************************+
 * Fichero: Transmissive.cpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#include "Transmissive.hpp"
#include <cmath>
#include "Transformation.hpp"

Transmissive::Transmissive(const RGB &kpr, const float ior) : BRDF(RGB(.0, .0, .0), RGB(.0, .0, .0), RGB(.0, .0, .0), kpr),
                                                              idx_of_refraction(ior) {}

RGB Transmissive::get_outgoing_sample_ray(const Ray &ri, const Direction &n, Ray &ro) const
{
    ro.set_origin(ri.get_position());
    ro.set_parameter(INFINITY);

    // Calcular probabilidades y normalizarlas
    float pkpr = kpr.max();
    float err = get_random_value(0.0f, 1.0f);
    if (err < pkpr)
    {
        // Refracción perfecta
        Direction out_direction = get_refraction(n, ri.get_direction(), idx_of_refraction);
        // Total internal reflection
        if (out_direction.mod() == 0)
            out_direction = get_reflection(n * -1, ri.get_direction());
        ro.set_direction(out_direction);

        return kpr / pkpr;
    }
    else
    {
        //MATAR RAYO
        return RGB(-1, -1, -1);
    }
}

RGB Transmissive::get_difusse() const
{
    return kd;
}

RGB Transmissive::get_specular() const
{
    return ks;
}

RGB Transmissive::get_perfect_specular() const
{
    return kps;
}

RGB Transmissive::get_perfect_refractive() const
{
    return kpr;
}

RGB Transmissive::get_fr(const Ray &ri, const Direction &n, const Ray &ro) const
{
    float ior_in = AIR_REFRACTION_INDEX;
    float ior_out = idx_of_refraction;
    float n_wi = dot(n, ri.get_direction());
    // Comprobaciones para ver si el rayo va desde el exterior al interior o viceversa
    // y aplicar la ecuación correctamente
    if (n_wi >= 0)
    {
        // hay que cambiar n de sentido y los
        // indices de refracción
        float aux = ior_in;
        ior_in = ior_out;
        ior_out = aux;
    }
    float ior = ior_in / ior_out;
    return kpr * ior * ior / fabs(dot(ro.get_direction(), n));
}

bool Transmissive::is_delta() const
{
    return true;
}