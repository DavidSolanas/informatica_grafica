/****************************************+
 * Fichero: Texture.cpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#include "../smallrt/include/Texture.h"

std::vector<std::vector<Vector3>> Texture::load_texture(std::string filename)
{
    std::ifstream f(filename);
    std::string str;
    int width, height, cr;
    std::vector<std::vector<Vector3>> data;

    if (f.is_open())
    {
        getline(f, str);
        getline(f, str);
        while (str[0] == '#')
        {
            getline(f, str);
        }
        //comment contendrá width y height
        width = std::stoi(str.substr(0, str.find(' ')));
        height = std::stoi(str.substr(str.find(' ') + 1, str.length()));
        f >> cr;
        int _r, _g, _b;
        data.resize(height, std::vector<Vector3>(width));
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                //Procesar RGB
                f >> _r >> _g >> _b;
                data[i][j][0] = (float)_r / cr;
                data[i][j][1] = (float)_g / cr;
                data[i][j][2] = (float)_b / cr;
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open the image, cancelling..." << std::endl;
    }
    return data;
}

Texture::Texture(World *_w, const Vector3 &_Kd, std::string texture_name) : Lambertian(_w, _Kd)
{
    texture = load_texture(texture_name);
}

void Texture::get_outgoing_sample_ray(const Intersection &it, Ray &r, Real &pdf) const
{
    Lambertian::get_outgoing_sample_ray(it, r, pdf);
}

Vector3 Texture::get_albedo(const Intersection &it) const
{
    Vector2 uv = it.get_uv();
    int j = uv[0] * texture[0].size();
    int i = (1 - uv[1]) * texture.size();

    if (j < 0)
        j = 0;
    if (i < 0)
        i = 0;

    if (j > texture[0].size() - 1)
        j = texture[0].size() - 1;
    if (i > texture.size() - 1)
        i = texture.size() - 1;
    return texture[i][j];
}

Real Texture::get_specular(const Intersection &it) const
{
    return Lambertian::get_specular(it);
}

bool Texture::is_delta() const
{
    return Lambertian::is_delta();
}