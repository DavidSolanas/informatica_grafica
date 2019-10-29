
/****************************************+
 * Fichero: Tone_mapper.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Tone_mapping.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

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

Image::Image(std::string fileId, std::vector<std::string> comments, int max, int width, int height,
             int cr, std::vector<std::vector<RGB>> data)
{
    this->fileId = fileId;
    this->comments = comments;
    this->MAX = max;
    this->width = width;
    this->height = height;
    this->color_res = cr;
    this->data = data;
}

std::string Image::getFileId()
{
    return this->fileId;
}

std::vector<std::string> Image::getComments()
{
    return this->comments;
}

int Image::getMax()
{
    return this->MAX;
}

int Image::getWidth()
{
    return this->width;
}

int Image::getHeight()
{
    return this->height;
}

int Image::getColorRes()
{
    return this->color_res;
}

std::vector<std::vector<RGB>> &Image::getData()
{
    return this->data;
}

Image load_HDR_image(std::string filename)
{
    std::ifstream f(filename);
    std::string fileId;
    int max, width, height, cr;
    std::vector<std::vector<RGB>> data;
    std::vector<std::string> comments;

    if (f.is_open())
    {
        getline(f, fileId);
        std::string comment;
        getline(f, comment);
        while (comment[0] == '#')
        {
            comments.push_back(comment);
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
                data[i][j].setR(((float)r / cr) * max);
                data[i][j].setG(((float)g / cr) * max);
                data[i][j].setB(((float)b / cr) * max);
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open the image, cancelling..." << std::endl;
    }
    return Image(fileId, comments, max, width, height, cr, data);
}

void save_LDR_image(std::string filename, int c, Image img)
{
    std::ofstream f(filename);
    if (f.is_open())
    {
        std::vector<std::vector<RGB>> data = img.getData();
        f << img.getFileId() << "\n";
        //for (std::string c : img.getComments())
        //    f << c << "\n";
        f << img.getWidth() << " " << img.getHeight() << "\n";
        f << c << "\n";
        for (int i = 0; i < data.size(); i++)
        {
            for (int j = 0; j < data[i].size(); j++)
            {
                f << round((data[i][j].getR() * c)) << " "
                  << round((data[i][j].getG() * c)) << " "
                  << round((data[i][j].getB() * c)) << "     ";
            }
            f << "\n";
        }
        f.close();
    }
    else
    {
        std::cerr << "Couldn't open the file, cancelling..." << std::endl;
    }
}

void clamping(Image &img)
{
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            rgb.setR(rgb.getR() < img.getMax() ? rgb.getR() : img.getMax());
            rgb.setG(rgb.getG() < img.getMax() ? rgb.getG() : img.getMax());
            rgb.setB(rgb.getB() < img.getMax() ? rgb.getB() : img.getMax());
        }
    }
}

void eq_clamp(Image &img, float V)
{
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            rgb.setR(rgb.getR() < V ? rgb.getR() : img.getMax());
            rgb.setG(rgb.getG() < V ? rgb.getG() : img.getMax());
            rgb.setB(rgb.getB() < V ? rgb.getB() : img.getMax());
        }
    }
}

void normalize(Image &img)
{
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            rgb.setR(rgb.getR() / img.getMax());
            rgb.setG(rgb.getG() / img.getMax());
            rgb.setB(rgb.getB() / img.getMax());
        }
    }
}

void gamma_encoding(Image &img, const float gamma)
{
    normalize(img);
    //float Vin, Vout;
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            //Vin = 0.2126 * rgb.getR() + 0.7152 * rgb.getG() + 0.0722 * rgb.getB();
            //Vout = pow(Vin, 1 / 2.2);
            rgb.setR(powf(rgb.getR(), 1 / gamma));
            rgb.setG(powf(rgb.getG(), 1 / gamma));
            rgb.setB(powf(rgb.getB(), 1 / gamma));
        }
    }
}

void clamp_gamma_encoding(Image &img, const float V, const float gamma)
{
    normalize(img);
    //float Vin, Vout;
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            //Vin = 0.2126 * rgb.getR() + 0.7152 * rgb.getG() + 0.0722 * rgb.getB();
            //Vout = pow(Vin, 1 / 2.2);
            rgb.setR(rgb.getR() < V ? powf(rgb.getR(), 1 / gamma) : 1);
            rgb.setG(rgb.getG() < V ? powf(rgb.getG(), 1 / gamma) : 1);
            rgb.setB(rgb.getB() < V ? powf(rgb.getB(), 1 / gamma) : 1);
        }
    }
}