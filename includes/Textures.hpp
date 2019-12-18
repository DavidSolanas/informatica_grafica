/****************************************+
 * Fichero: Textures.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include "RGB.hpp"
#include <vector>
#include <string>

std::vector<std::vector<RGB>> load_texture(std::string filename);

RGB get_pixel(std::vector<std::vector<RGB>> &data, const float u, const float v);

#endif // !TEXTURES_HPP