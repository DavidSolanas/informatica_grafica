
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
    int max, width, height, cr;
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
        float r, g, b;
        data.resize(height, std::vector<RGB>(width));
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                //Procesar RGB
                f >> r >> g >> b;
                r /= cr;
                g /= cr;
                b /= cr;
                if (r > maxPixelValue)
                    maxPixelValue = r;
                if (g > maxPixelValue)
                    maxPixelValue = g;
                if (b > maxPixelValue)
                    maxPixelValue = b;
                data[i][j].setR(r);
                data[i][j].setG(g);
                data[i][j].setB(b);
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open the image, cancelling..." << std::endl;
    }
    return Image(fileId, comments, max, width, height, cr, maxPixelValue, data);
}

void save_LDR_image(std::string filename, int c, Image img)
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
            rgb.setR(rgb.getR() < 1.0f ? rgb.getR() : 1.0f);
            rgb.setG(rgb.getG() < 1.0f ? rgb.getG() : 1.0f);
            rgb.setB(rgb.getB() < 1.0f ? rgb.getB() : 1.0f);
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
            rgb.setR(rgb.getR() < V ? rgb.getR() : 1.0f);
            rgb.setG(rgb.getG() < V ? rgb.getG() : 1.0f);
            rgb.setB(rgb.getB() < V ? rgb.getB() : 1.0f);
        }
    }
}

void equalize(Image &img, const float V)
{
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            rgb.setR((float)(rgb.getR() / V));
            rgb.setG((float)(rgb.getG() / V));
            rgb.setB((float)(rgb.getB() / V));
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
            rgb.setR(powf(rgb.getR(), gamma));
            rgb.setG(powf(rgb.getG(), gamma));
            rgb.setB(powf(rgb.getB(), gamma));
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
            rgb.setR(rgb.getR() < V ? powf(rgb.getR(), gamma) : 1.0f);
            rgb.setG(rgb.getG() < V ? powf(rgb.getG(), gamma) : 1.0f);
            rgb.setB(rgb.getB() < V ? powf(rgb.getB(), gamma) : 1.0f);
        }
    }
}

float equation_1_reinhard(Image img, const float delta)
{
    float sum = 0.0;
    const int N = img.getHeight() * img.getWidth();
    float L;
    for (auto &&row : img.getData())
    {
        for (auto &&rgb : row)
        {
            L = 0.27 * rgb.getR() + 0.67 * rgb.getG() + 0.06 * rgb.getB();
            sum += log(delta + L);
        }
    }
    return exp((1.0f / N) * sum);
}

float equation_2_reinhard(Image img, std::vector<std::vector<float>> &vl, const float key)
{
    float worldAVG_L = equation_1_reinhard(img, 0.0001f);
    float L, Lmax = -1.0f;
    int i = 0, j = 0;
    for (auto &&row : img.getData())
    {
        j = 0;
        for (auto &&rgb : row)
        {
            L = 0.27 * rgb.getR() + 0.67 * rgb.getG() + 0.06 * rgb.getB();
            vl[i][j] = (key / worldAVG_L) * L;
            if (vl[i][j] > Lmax)
                Lmax = vl[i][j];
            j++;
        }
        i++;
    }
    return Lmax;
}

std::vector<std::vector<float>> equation_4_reinhard(std::vector<std::vector<float>> vl, const float l_white)
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
            float L = 0.27 * rgb.getR() + 0.67 * rgb.getG() + 0.06 * rgb.getB();
            rgb.setR(rgb.getR() * (vld[i][j] / L));
            rgb.setG(rgb.getG() * (vld[i][j] / L));
            rgb.setB(rgb.getB() * (vld[i][j] / L));
            j++;
        }
        i++;
    }
}

void equation_5_reinhard(std::vector<std::vector<float>> &Ri, const float scale, const float alphai)
{
    for (int i = 0; i < Ri.size(); i++)
    {
        for (int j = 0; j < Ri[i].size(); j++)
        {
            Ri[i][j] = 1.0f / (M_PI * ((alphai * scale) * (alphai * scale)));
            Ri[i][j] *= exp(-((i * i) + (j * j)) / ((alphai * scale) * (alphai * scale)));
        }
    }
}

float equation_5_reinhardV2(const int x, const int y, const float scale, const float alphai)
{
    float res = 1.0f / (M_PI * ((alphai * scale) * (alphai * scale)));
    res *= exp(-((x * x) + (y * y)) / ((alphai * scale) * (alphai * scale)));
    return res;
}

/* https://rosettacode.org/wiki/Kronecker_product */
std::vector<std::vector<float>> kronecker_product(std::vector<std::vector<float>> A, std::vector<std::vector<float>> B)
{
    std::vector<std::vector<float>> C(A.size() * B.size(), std::vector<float>(A[0].size() * B[0].size()));

    int nRowA = A.size(), nColA = A[0].size(),
        nRowB = B.size(), nColB = B[0].size();

    for (int i = 0; i < nRowA; i++)
    {
        for (int j = 0; j < nColA; j++)
        {
            int startRow = i * nRowB;
            int startCol = j * nColB;
            for (int k = 0; k < nRowB; k++)
            {
                for (int l = 0; l < nColB; l++)
                {
                    C[startRow + k][startCol + l] = A[i][j] * B[k][l];
                }
            }
        }
    }
    return C;
}

std::vector<std::vector<float>> equation_6_reinhard(std::vector<std::vector<float>> L, std::vector<std::vector<float>> Ri)
{
    int h = L.size(), w = L[0].size();
    std::vector<std::vector<float>> V(h, std::vector<float>(w));
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            V[i][j] = L[i][j] * Ri[i][j];
        }
    }
    return V;
    //return kronecker_product(L, Ri);
}

float equation_6_reinhardV2(const float L, const float Ri)
{
    return L * Ri;
}

std::vector<std::vector<float>> equation_7_reinhard(Image img, std::vector<std::vector<float>> L, const float scale,
                                                    const float phi = 8.0f, const float key = 0.18f)
{
    int h = img.getHeight(), w = img.getWidth();
    std::vector<std::vector<float>> R1(h, std::vector<float>(w));
    equation_5_reinhard(R1, scale, M_SQRT2 / 4);
    std::vector<std::vector<float>> R2(h, std::vector<float>(w));
    equation_5_reinhard(R2, scale * 1.6f, 2 * M_SQRT2 / 5);

    std::vector<std::vector<float>> V1 = equation_6_reinhard(L, R1);
    std::vector<std::vector<float>> V2 = equation_6_reinhard(L, R2);
    std::vector<std::vector<float>> V(h, std::vector<float>(w));
    float p = powf(2.0f, phi) * key / (scale * scale);

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            V[i][j] = (V1[i][j] - V2[i][j]) / (p + V1[i][j]);
        }
    }
    return V;
}

float equation_7_reinhardV2(const int x, const int y, const float Lxy, const float scale,
                            const float phi = 8.0f, const float key = 0.18f)
{
    float R1 = equation_5_reinhardV2(x, y, scale, M_SQRT2 / 4);
    float R2 = equation_5_reinhardV2(x, y, scale * 1.6f, 2 * M_SQRT2 / 5);

    float V1 = equation_6_reinhardV2(Lxy, R1);
    float V2 = equation_6_reinhardV2(Lxy, R2);
    float norm = powf(2.0f, phi) * key / (scale * scale);

    return (V1 - V2) / (norm + V1);
}

std::vector<std::vector<float>> equation_8_reinhard(std::vector<std::vector<float>> L, const float lowScale,
                                                    const float highScale, const float phi, const float key,
                                                    const float threshold, const int numScales)
{
    int h = L.size(), w = L[0].size();
    std::vector<std::vector<float>> Sm(h, std::vector<float>(w));

    float factor = exp((log(highScale) / numScales));
    for (int x = 0; x < h; x++)
    {
        for (int y = 0; y < w; y++)
        {
            float sm = lowScale;
            for (int k = 0; k < numScales; k++)
            {
                float vm = equation_7_reinhardV2(x, y, L[x][y], sm, phi, key);
                if (abs(vm) < threshold)
                {
                    Sm[x][y] = sm;
                    break;
                }
                else
                {
                    sm *= factor;
                }
            }
        }
    }
    return Sm;
}

std::vector<std::vector<float>> equation_9_reinhard(std::vector<std::vector<float>> Sm,
                                                    std::vector<std::vector<float>> L)
{
    int h = L.size(), w = L[0].size();
    std::vector<std::vector<float>> Ld(h, std::vector<float>(w));
    for (int x = 0; x < h; x++)
    {
        for (int y = 0; y < w; y++)
        {
            float Ri = equation_5_reinhardV2(x, y, Sm[x][y], M_SQRT2 / 4);
            float Vi = equation_6_reinhardV2(L[x][y], Ri);
            Ld[x][y] = L[x][y] / (1 + Vi);
        }
    }
    return Ld;
}

void local_reinhard(Image &img, const float lowScale, const float highScale,
                    const float phi, const float key, const float threshold, const int numScales)
{
    int h = img.getHeight(), w = img.getWidth();
    std::vector<std::vector<float>> L(h, std::vector<float>(w));
    equation_2_reinhard(img, L, key);
    std::vector<std::vector<float>> Sm = equation_8_reinhard(L, lowScale, highScale, phi, key, threshold, numScales);
    std::vector<std::vector<float>> Ld = equation_9_reinhard(Sm, L);

    int x = 0, y = 0;
    for (auto &&row : img.getData())
    {
        y = 0;
        for (auto &&rgb : row)
        {
            float L = 0.27 * rgb.getR() + 0.67 * rgb.getG() + 0.06 * rgb.getB();
            rgb.setR(rgb.getR() * (Ld[x][y] / L));
            rgb.setG(rgb.getG() * (Ld[x][y] / L));
            rgb.setB(rgb.getB() * (Ld[x][y] / L));
            y++;
        }
        x++;
    }
}