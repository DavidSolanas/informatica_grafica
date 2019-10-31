
/****************************************+
 * Fichero: Tone_mapper.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef TONE_MAPPING_HPP
#define TONE_MAPPING_HPP

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
    std::vector<std::string> comments;
    int MAX;
    int width;
    int height;
    int color_res;
    float maxPixelValue;
    std::vector<std::vector<RGB>> data;

public:
    Image(std::string fileId, std::vector<std::string> comments, int max, int width, int height,
          int cr, float maxPixelValue, std::vector<std::vector<RGB>> data);
    std::string getFileId();
    std::vector<std::string> getComments();
    int getMax();
    int getWidth();
    int getHeight();
    int getColorRes();
    float getMaxPixelValue();
    std::vector<std::vector<RGB>> &getData();
};

Image load_HDR_image(std::string filename);

void save_LDR_image(std::string filename, int c, Image img);

void clamping(Image &img);

void eq_clamp(Image &img, float V);

void equalize(Image &img, const float V);

void gamma_encoding(Image &img, const float gamma = 1.6f);

void clamp_gamma_encoding(Image &img, const float V, const float gamma = 1.6f);

void global_reinhard(Image &img, const float key = 0.18f);

#endif // !TONE_MAPPING_HPP