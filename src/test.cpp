#include <iostream>
#include <cmath>
#include <fstream>
#include "Transformation.hpp"
#include "Sphere.hpp"
#include "Tone_mapping.hpp"

int main(int argc, const char **argv)
{
    Image img = load_HDR_image("../media/hdr-ppm/mpi_office.ppm");
    Image img2 = load_HDR_image("../media/hdr-ppm/mpi_office.ppm");
    //clamping(img);
    //eq_clamp(img, 45000.0);

    gamma_encoding(img);
    clamp_gamma_encoding(img2, 0.7);

    save_LDR_image("./testimage.ppm", 255, img);
    save_LDR_image("./testimage2.ppm", 255, img2);
    return 0;
}