#include <iostream>
#include "Transformation.hpp"
#include <cmath>

int main(int argc, const char **argv)
{
    Direction d(2, 4, 6);
    Point p(3, 7, 9);

    d.view();
    translation(d, 20, 10, 15).view();
    std::cout << "--------------" << std::endl;
    p.view();
    translation(p, 20, 10, 15).view();

    std::cout << std::endl;

    d.view();
    scale(d, 5, 10, 100).view();
    std::cout << "--------------" << std::endl;
    p.view();
    scale(p, 5, 10, 100).view();

    std::cout << std::endl;

    d.view();
    rotate(d, M_PI, 0).view();
    std::cout << "--------------" << std::endl;
    rotate(d, M_PI, 1).view();
    std::cout << "--------------" << std::endl;
    rotate(d, M_PI, 2).view();
    std::cout << "--------------" << std::endl;

    std::cout << std::endl;

    p.view();
    rotate(p, M_PI, 0).view();
    std::cout << "--------------" << std::endl;
    rotate(p, M_PI, 1).view();
    std::cout << "--------------" << std::endl;
    rotate(p, M_PI, 2).view();

    std::cout << std::endl;

    d.view();
    Direction u(5, 5, 5);
    Direction v(1, 8, 10);
    Direction w(11, 12, 13);
    Point p1(0, 7, 25);
    change_base(d, p1, u, v, w).view();
    std::cout << "--------------" << std::endl;
    p.view();
    change_base(p, p1, u, v, w).view();

    return 0;
}