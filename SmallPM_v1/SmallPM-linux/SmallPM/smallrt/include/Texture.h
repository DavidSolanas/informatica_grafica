/*********************************************************************************
Copyright (C) 2014 Adrian Jarabo (ajarabo@unizar.es)
Copyright (C) 2014 Diego Gutierrez (diegog@unizar.es)
All rights reserved.

This is an educational Ray Tracer developed for the course 'Informatica Grafica'
(Computer Graphics) tought at Universidad de Zaragoza (Spain). As such, it does not 
intend to be fast or general, but just to provide an educational tool for undergraduate
students. 

This software is provided as is, and any express or implied warranties are disclaimed.
In no event shall copyright holders be liable for any damage.
**********************************************************************************/

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