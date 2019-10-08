#include <iostream>
#include "Transformation.hpp"
#include "Planets.hpp"
#include <cmath>

int main(int argc, const char **argv)
{

    Point c1(-100, 0, 50);
    Direction ax(0, 0, 2);
    Point city1(-99, 0, 50);
    Planet planet1(c1, ax, city1);
    Station s1(2.432436, 1.543534654, planet1);

    Point c2(0, 0, 0);
    Point city2(-1, 0, 0);
    Planet planet2(c2, ax, city2);
    Station s2(0.543234544, -2.4354654, planet2);
    Direction RAYO_LASER = s1.getPosition() - s2.getPosition();
    RAYO_LASER.view();
    rotate(RAYO_LASER, M_PI_2, 1).view();
    Direction u(0, 0, -1);
    Direction v(0, 1, 0);
    Direction w(1, 0, 0);
    change_base(RAYO_LASER, c2, u, v, w).view();
    change_base(RAYO_LASER, Point(100, 0, -50), Direction(-1, 0, 0), v, Direction(0, 0, -1)).view();
    /*
    Direction i = s.getLongitudeTD();
    Direction j = s.getLatitudeTD();
    Direction k = s.getNormal();
    s.getPosition().view();
    std::cout << std::endl;
    i.view();
    j.view();
    k.view();
    std::cout << i.mod() << std::endl;
    std::cout << j.mod() << std::endl;
    std::cout << k.mod() << std::endl;
    std::cout << dot(i, j) << std::endl;
    std::cout << dot(i, k) << std::endl;
    std::cout << dot(j, k) << std::endl;
    std::cout << std::endl;
    rotate(city, M_PI_2, 2).view();

    change_base(city, c, v, Direction(-1, 0, 0), w).view();
    */
    return 0;
}