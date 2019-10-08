#include <iostream>
#include "Transformation.hpp"
#include "Sphere.hpp"
#include <cmath>

int main(int argc, const char **argv)
{
    /*
    Point c1(-100, 0, 50);
    Direction ax(0, 2, 0);
    Point city1(-99, 0, 50);
    Sphere Sphere1(c1, ax, city1);
    SphereGeometry s1(2.432436, 1.543534654, Sphere1);

    Point c2(0, 0, 0);
    Point city2(1, 0, 0);
    Sphere Sphere2(c2, ax, city2);
    SphereGeometry s2(M_PI_2, 0, Sphere2);
    /
    Direction RAYO_LASER = s1.getPosition() - s2.getPosition();
    RAYO_LASER.view();
    rotate(RAYO_LASER, M_PI_2, 1).view();
    Direction u(0, 0, -1);
    Direction v(0, 1, 0);
    Direction w(1, 0, 0);
    change_base(RAYO_LASER, c2, u, v, w).view();
    change_base(RAYO_LASER, Point(100, 0, -50), Direction(-1, 0, 0), v, Direction(0, 0, -1)).view();
    */
    /*
    Direction i = s2.getLongitudeTD();
    Direction j = s2.getLatitudeTD();
    Direction k = s2.getNormal();
    s2.getPosition().view();
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
    //rotate(city, M_PI_2, 2).view();

    //change_base(city, c, v, Direction(-1, 0, 0), w).view();
*/
    Point c1(0, 0, 0);
    Direction ax(0, 2, 0);
    Point city1(0, 0, -1);
    Sphere p1(c1, ax, city1);
    SphereGeometry s1(M_PI_2, -M_PI_2, p1);

    Point c2(-100, 50, 0);
    Point city2(-99, 50, 0);
    Sphere p2(c2, ax, city2);
    SphereGeometry s2(M_PI_2, 0, p2);
    Direction RAYO_LASER = s2.getPosition() - s1.getPosition();
    s1.getPosition().view();
    s2.getPosition().view();
    RAYO_LASER.view();
    std::cout << RAYO_LASER.mod() << std::endl;
    return 0;
}