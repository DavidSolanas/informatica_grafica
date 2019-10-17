
/****************************************+
 * Fichero: Tone_mapper.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef TONE_MAPPER_HPP
#define TONE_MAPPER_HPP

#include <vector>
#include <string>

class RGB
{
private:
    float r, g, b;

public:
    RGB();
    RGB(float r, float g, float b);
    void setR(float r);
    void setG(float g);
    void setB(float b);
    float getR();
    float getG();
    float getB();
};

class Image
{
private:
    std::string fileId;
    int MAX;
    int width;
    int height;
    int color_res;
    std::vector<std::vector<RGB>> data;

public:
    Image(std::string fileId, int max, int width, int height,
          int cr, std::vector<std::vector<RGB>> data);
    std::vector<std::vector<RGB>> getData();
};

Image read_img(std::string filename);

#endif // !TONE_MAPPER_HPP