
/****************************************+
 * Fichero: Tone_mapper.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Tone_mapping.hpp"
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

void save_ppm_image(std::string filename, Image img)
{
    std::ofstream f(filename);
    if (f.is_open())
    {
        std::vector<std::vector<RGB>> data = img.getData();
        f << img.getFileId() << "\n";
        for (std::string c : img.getComments())
            f << c << "\n";
        f << img.getWidth() << " " << img.getHeight() << "\n";
        f << img.getColorRes() << "\n";
        for (int i = 0; i < data.size(); i++)
        {
            for (int j = 0; j < data[i].size(); j++)
            {
                f << data[i][j].getR() << " " << data[i][j].getG() << " " << data[i][j].getB() << "     ";
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