#include <iostream>
#include <cmath>
#include <fstream>
#include "Transformation.hpp"
#include "Sphere.hpp"
#include "Tone_mapping.hpp"

int main(int argc, const char **argv)
{
    Image img = load_HDR_image("../media/hdr-ppm/seymour_park.ppm");
    //Image img2 = load_HDR_image("../media/hdr-ppm/seymour_park.ppm");
    //Image img3 = load_HDR_image("../media/hdr-ppm/seymour_park.ppm");
    //clamping(img);
    //equalize(img, img.getMaxPixelValue());
    //eq_clamp(img, 0.8);
    //equation_1_reinhard(img, 0.0001);
    //gamma_encoding(img);
    //gamma_encoding(img2, 1.6f);
    //gamma_encoding(img3, 1.8f);
    //clamp_gamma_encoding(img, 0.85);

    global_reinhard(img);

    save_LDR_image("/Users/david/Desktop/ldr_seymour_park.ppm", 65535, img);
    //save_LDR_image("/Users/david/Desktop/ldr_seymour_park2.ppm", 65535, img2);
    //save_LDR_image("/Users/david/Desktop/ldr_seymour_park3.ppm", 65535, img3);
    return 0;
}