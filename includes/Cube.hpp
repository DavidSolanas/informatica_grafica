
/****************************************+
 * Fichero: Cube.hpp
 * Autor: David Solanas Sanz    738630
 *****************************************/

#ifndef CUBE_HPP
#define CUBE_HPP

#include "Plane.hpp"

class Cube : public Object
{
public:
     BoundedPlane front;
     BoundedPlane back;
     BoundedPlane left;
     BoundedPlane right;
     BoundedPlane up;
     BoundedPlane down;

     Cube(BoundedPlane _front, BoundedPlane _back,
          BoundedPlane _left, BoundedPlane _right,
          BoundedPlane _up, BoundedPlane _down,
          BRDF *mat) : Object(mat), front(_front), back(_back), left(_left), right(_right), up(_up), down(_down) {}

     Cube(BoundedPlane _front, BoundedPlane _back,
          BoundedPlane _left, BoundedPlane _right,
          BoundedPlane _up, BoundedPlane _down,
          BRDF *mat, bool texture, std::string texture_file) : Object(mat, texture, texture_file), front(_front), back(_back), left(_left), right(_right), up(_up), down(_down) {}

     ~Cube() {}

     Direction getNormal(Point X) override;
     bool intersect(Ray &ray) override;
     float get_area() override;
     void get_uv(const Point &X, const Direction &n, float &u, float &v) override;
};

#endif // !CUBE_HPP