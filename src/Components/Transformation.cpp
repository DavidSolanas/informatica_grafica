/****************************************+
 * Fichero: Transformation.cpp
 * Autor: David Solanas
 *****************************************/

#include "Transformation.hpp"
#include <cmath>

Point translation(const Point &p, const float x,
                  const float y, const float z)
{
}

Direction translation(const Direction &d, const float x,
                      const float y, const float z) {}

Point scale(const Point &p, const float x,
            const float y, const float z) {}

Direction scale(const Direction &d, const float x,
                const float y, const float z) {}

Point rotate(const Point &p, const float theta, const int axis) {}

Direction rotate(const Direction &d, const float theta, const int axis) {}

Point change_base(const Point &p, const Point &origin, const Direction &u,
                  const Direction &v, const Direction &w) {}

Direction change_base(const Direction &p, const Point &origin, const Direction &u,
                      const Direction &v, const Direction &w) {}