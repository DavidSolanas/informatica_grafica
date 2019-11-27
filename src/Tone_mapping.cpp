
/****************************************+
 * Fichero: Tone_mapper.cpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#include "Tone_mapping.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstring>

Image::Image(std::string fileId, std::vector<std::string> comments, int max, int width, int height,
             int cr, float maxPixelValue, std::vector<std::vector<RGB>> data)
{
    this->fileId = fileId;
    this->comments = comments;
    this->MAX = max;
    this->width = width;
    this->height = height;
    this->color_res = cr;
    this->maxPixelValue = maxPixelValue;
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

float Image::getMaxPixelValue()
{
    return this->maxPixelValue;
}

std::vector<std::vector<RGB>> &Image::getData()
{
    return this->data;
}

Image load_HDR_image(std::string filename)
{
    std::ifstream f(filename);
    std::string fileId;
    int max = 0, width = 0, height = 0, cr = 0;
    float maxPixelValue = -1.0f;
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
        float _r, _g, _b;
        data.resize(height, std::vector<RGB>(width));
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                //Procesar RGB
                f >> _r >> _g >> _b;
                _r /= cr;
                _g /= cr;
                _b /= cr;
                if (_r > maxPixelValue)
                    maxPixelValue = _r;
                if (_g > maxPixelValue)
                    maxPixelValue = _g;
                if (_b > maxPixelValue)
                    maxPixelValue = _b;
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
    return Image(fileId, comments, max, width, height, cr, maxPixelValue, data);
}

void save_LDR_image(std::string filename, int c, Image &img)
{
    std::ofstream f(filename);
    if (f.is_open())
    {
        std::vector<std::vector<RGB>> data = img.getData();
        f << img.getFileId() << "\n";
        for (std::string c : img.getComments())
            f << c << "\n";
        f << img.getWidth() << " " << img.getHeight() << "\n";
        f << c << "\n";
        for (int i = 0; i < data.size(); i++)
        {
            for (int j = 0; j < data[i].size(); j++)
            {
                f << round((data[i][j].r * c)) << " "
                  << round((data[i][j].g * c)) << " "
                  << round((data[i][j].b * c)) << "     ";
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
            rgb.r = rgb.r < 1.0f ? rgb.r : 1.0f;
            rgb.g = rgb.g < 1.0f ? rgb.g : 1.0f;
            rgb.b = rgb.b < 1.0f ? rgb.b : 1.0f;
        }
    }
}

void eq_clamp(Image &img, float V)
{
    equalize(img, V);
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            rgb.r = rgb.r < V ? rgb.r : 1.0f;
            rgb.g = rgb.g < V ? rgb.g : 1.0f;
            rgb.b = rgb.b < V ? rgb.b : 1.0f;
        }
    }
}

void equalize(Image &img, const float V)
{
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            rgb.r = (float)(rgb.r / V);
            rgb.g = (float)(rgb.g / V);
            rgb.b = (float)(rgb.b / V);
        }
    }
}

void gamma_encoding(Image &img, const float gamma)
{
    equalize(img, img.getMaxPixelValue());
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            rgb.r = powf(rgb.r, gamma);
            rgb.g = powf(rgb.g, gamma);
            rgb.b = powf(rgb.b, gamma);
        }
    }
}

void clamp_gamma_encoding(Image &img, const float V, const float gamma)
{
    equalize(img, V);
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            rgb.r = rgb.r < V ? powf(rgb.r, gamma) : 1.0f;
            rgb.g = rgb.g < V ? powf(rgb.g, gamma) : 1.0f;
            rgb.b = rgb.b < V ? powf(rgb.b, gamma) : 1.0f;
        }
    }
}

float equation_1_reinhard(Image &img, const float delta)
{
    float sum = 0.0;
    const int N = img.getHeight() * img.getWidth();
    float L;
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            L = 0.27 * rgb.r + 0.67 * rgb.g + 0.06 * rgb.b;
            sum += log(delta + L);
        }
    }
    return exp((1.0f / N) * sum);
}

float equation_2_reinhard(Image &img, std::vector<std::vector<float>> &vl, const float key)
{
    float worldAVG_L = equation_1_reinhard(img, 0.0001f);
    float L, Lmax = -1.0f;
    int i = 0, j = 0;
    for (auto &&row : img.getData())
    {
        j = 0;
        for (auto &&rgb : row)
        {
            L = 0.27 * rgb.r + 0.67 * rgb.g + 0.06 * rgb.b;
            vl[i][j] = (key / worldAVG_L) * L;
            if (vl[i][j] > Lmax)
                Lmax = vl[i][j];
            j++;
        }
        i++;
    }
    return Lmax;
}

std::vector<std::vector<float>> equation_4_reinhard(std::vector<std::vector<float>> &vl, const float l_white)
{
    std::vector<std::vector<float>> vld(vl.size(), std::vector<float>(vl[0].size(), 0.0f));
    for (int i = 0; i < vl.size(); i++)
    {
        for (int j = 0; j < vl[i].size(); j++)
        {
            vld[i][j] = vl[i][j] * (1.0f + (vl[i][j] / (l_white * l_white)));
            vld[i][j] /= (1.0f + vl[i][j]);
        }
    }
    return vld;
}

void global_reinhard(Image &img, const float key)
{
    std::vector<std::vector<float>> vl(img.getHeight(), std::vector<float>(img.getWidth(), 0.0f));
    float l_max = equation_2_reinhard(img, vl, key);
    std::vector<std::vector<float>> vld = equation_4_reinhard(vl, l_max);
    int i = 0, j = 0;
    for (auto &&row : img.getData())
    {
        j = 0;
        for (auto &&rgb : row)
        {
            //Se añade 0.0000001f para evitar singularidades cuando RGB = 000
            float L = 0.27 * rgb.r + 0.67 * rgb.g + 0.06 * rgb.b + 0.0000001f;
            rgb.r = rgb.r * (vld[i][j] / (1.0f + L));
            rgb.g = rgb.g * (vld[i][j] / (1.0f + L));
            rgb.b = rgb.b * (vld[i][j] / (1.0f + L));
            j++;
        }
        i++;
    }
}

int main(int argc, const char **argv)
{
    std::string usage = "Usage: Tone_mapping -i hdr_img -o ldr_img [-k key]";
    if (argc < 5)
    {
        std::cerr << "Incorrect number of parameters" << std::endl;
        std::cerr << usage << std::endl;
    }
    else
    {
        std::string input, output, f;
        float key = 0.18f;
        for (int i = 1; i < argc; i++)
        {
            switch (i)
            {
            case 1:
                if (strcmp("-i", argv[i]) != 0)
                {
                    std::cerr << "Incorrect parameter, found \"" << argv[i] << "\" but expected \"-i\"" << std::endl;
                    std::cerr << usage << std::endl;
                    exit(1);
                }
                break;
            case 2:
                input = argv[i];
                break;
            case 3:
                if (strcmp("-o", argv[i]) != 0)
                {
                    std::cerr << "Incorrect parameter, found \"" << argv[i] << "\" but expected \"-o\"" << std::endl;
                    std::cerr << usage << std::endl;
                    exit(1);
                }
                break;
            case 4:
                output = argv[i];
                break;
            case 5:
                if (strcmp("-k", argv[i]) != 0)
                {
                    std::cerr << "Incorrect parameter, found \"" << argv[i] << "\" but expected \"-k\"" << std::endl;
                    std::cerr << usage << std::endl;
                    exit(1);
                }
                break;
            case 6:
                f = argv[i];
                key = std::stof(f);
                if (key < 0.0f)
                {
                    std::cerr << "Key value must be positive, setting to 0.18" << std::endl;
                    key = 0.18f;
                }
                break;
            default:
                break;
            }
        }
        Image img = load_HDR_image(input);
        global_reinhard(img, key);
        clamping(img);
        save_LDR_image(output, 65535, img);
    }
    return 0;
}