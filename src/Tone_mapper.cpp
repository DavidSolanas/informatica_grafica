
/****************************************+
 * Fichero: Tone_mapper.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Tone_mapper.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

RGB::RGB()
{
}

RGB::RGB(float rv, float gv, float bv) : r(rv), g(gv), b(bv)
{
}

void RGB::setR(float r)
{
    this->r = r;
}

void RGB::setG(float g)
{
    this->g = g;
}

void RGB::setB(float b)
{
    this->b = b;
}

float RGB::getR()
{
    return this->r;
}

float RGB::getG()
{
    return this->g;
}

float RGB::getB()
{
    return this->b;
}

Image::Image(std::string fileId, int max, int width, int height,
             int cr, std::vector<std::vector<RGB>> data)
{
    this->fileId = fileId;
    this->MAX = max;
    this->width = width;
    this->height = height;
    this->color_res = cr;
    this->data = data;
}

std::vector<std::vector<RGB>> Image::getData()
{
    return this->data;
}

Image read_img(std::string filename)
{
    std::ifstream f(filename);
    std::string fileId;
    int max, width, height, cr;
    std::vector<std::vector<RGB>> data;

    if (f.is_open())
    {
        getline(f, fileId);
        std::string comment;
        getline(f, comment);
        while (comment[0] == '#')
        {
            if (comment[1] == 'M')
            {
                max = std::stoi(comment.substr(comment.find('=') + 1, comment.length()));
            }
            getline(f, comment);
        }
        //comment contendrá width y height
        width = std::stoi(comment.substr(0, comment.find(' ')));
        height = std::stoi(comment.substr(comment.find(' ') + 1, comment.length()));
        f >> cr;
        int r, g, b;
        data.resize(height, std::vector<RGB>(width));
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                //Procesar RGB
                f >> r >> g >> b;
                data[i][j].setR(r * ((float)max / cr));
                data[i][j].setG(g * ((float)max / cr));
                data[i][j].setB(b * ((float)max / cr));
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open the image, cancelling..." << std::endl;
    }
    return Image(fileId, max, width, height, cr, data);
}