/****************************************+
 * Fichero: Ray_tracer.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include <fstream>
#include "Scene.hpp"

void ray_tracer(std::string filename, Camera c, const int W, const int H);

#endif // !RAY_TRACER_HPP