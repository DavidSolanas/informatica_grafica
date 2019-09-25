#include <iostream>
#include "Geometry.hpp"

int main(int argc, const char **argv)
{
    float x, y, z;
    std::cin >> x >> y >> z;
    Direction d(x, y, z);
    Direction rnd(3, 2, 1);
    Direction e = corss(d, rnd);
    auto ce = e.getCoord();
    std::cout << ce[0] << " " << ce[1] << " " << ce[2] << " " << ce[3] << std::endl;
    e = d + e;
    ce = e.getCoord();
    std::cout << ce[0] << " " << ce[1] << " " << ce[2] << " " << ce[3] << std::endl;
    std::cout << dot(d, e) << std::endl;
    std::cout << d.mod() << std::endl;
    std::cout << std::endl;
    Point p1(1, 2, 3);
    auto pp = p1.getCoord();
    std::cout << pp[0] << " " << pp[1] << " " << pp[2] << " " << pp[3] << std::endl;
    Point p2(5, 6, 7);
    auto pc = d + p1;
    pp = pc.getCoord();
    std::cout << pp[0] << " " << pp[1] << " " << pp[2] << " " << pp[3] << std::endl;
    return 0;
}