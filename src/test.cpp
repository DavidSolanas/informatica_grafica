#include <iostream>
#include <cmath>
#include <fstream>
#include "Transformation.hpp"
#include "Sphere.hpp"
#include "Tone_mapping.hpp"

int main(int argc, const char **argv)
{
    Image img = read_img("../media/hdr-ppm/mpi_atrium_1.ppm");
    save_ppm_image("./testimage.ppm", img);
    return 0;
}