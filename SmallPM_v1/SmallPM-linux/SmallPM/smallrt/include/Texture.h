/****************************************+
 * Fichero: Texture.h
 * Autor: David Solanas Sanz    738630
 *****************************************/

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Lambertian.h"
#include <vector>
#include <fstream>
#include <iostream>

class Texture : public Lambertian
{
    //Diffuse coefficient
    std::vector<std::vector<Vector3>> texture;

    std::vector<std::vector<Vector3>> load_texture(std::string filename);

public:
    Texture(World *_w, const Vector3 &_Kd, std::string texture_name);

    void get_outgoing_sample_ray(const Intersection &it, Ray &r, Real &pdf) const override;
    Vector3 get_albedo(const Intersection &it) const override;
    Real get_specular(const Intersection &it) const override;
    bool is_delta() const override;
};

#endif