#include <iostream>
#include <cmath>
#include <fstream>
#include "Transformation.hpp"
#include "Sphere.hpp"
#include "Tone_mapping.hpp"

int main(int argc, const char **argv)
{
    Image img = load_HDR_image("../media/hdr-ppm/seymour_park.ppm");
    //clamping(img);
    eq_clamp(img, 45000.0);
    save_LDR_image("./testimage.ppm", 255, img);
    return 0;
}