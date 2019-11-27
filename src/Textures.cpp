/****************************************+
 * Fichero: Textures.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Textures.hpp"
#include <fstream>
#include <iostream>

std::vector<std::vector<RGB>> load_texture(std::string filename)
{
    std::ifstream f(filename);
    std::string str;
    int width, height, cr;
    std::vector<std::vector<RGB>> data;

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
        data.resize(height, std::vector<RGB>(width));
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                //Procesar RGB
                f >> _r >> _g >> _b;
                data[i][j].r = _r;
                data[i][j].g = _g;
                data[i][j].b = _b;
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open the image, cancelling..." << std::endl;
    }
    return data;
}

RGB get_pixel(std::vector<std::vector<RGB>> &data, const float u, const float v)
{
    int j = u * data[0].size();
    int i = (1 - v) * data.size();

    if (j < 0)
        j = 0;
    if (i < 0)
        i = 0;

    if (j > data[0].size() - 1)
        j = data[0].size() - 1;
    if (i > data.size() - 1)
        i = data.size() - 1;

    return data[i][j];
}