/****************************************+
 * Fichero: Russian_Roulette.hpp
 * Autor: David Solanas, Santiago Buey
 *****************************************/

#ifndef RUSSIAN_ROULETTE_HPP
#define RUSSIAN_ROULETTE_HPP

/* 
    Importance sampling algorithm:
    1) Choose N uniform random numbers Xi -> [0,1]
    2) Calculate all random samples xi = c^-1(Xi)
    3) Approximate the integral -> 1/N ∑ f(xi)/p(xi)

    UNIFORM COSINE SAMPLING:
    p(thi) = 2 * sin(thi) * cos(thi)
    c(thi) = sin(thi) * sin(thi)
    c^-1(Xthi) = acos(sqrt(1 - Xthi))
    p(phi) = 1 / 2π
    c(phi) = phi / 2π
    c^-1(Xphi) = 2* π * Xphi 

    PHONG SPECULAR LOBE SAMPLING:
    p(thr) = (alpha + 1) * cos(thr)^alpha * sin(thr)
    c(thr) = cos(thr)^(alpha+1) 
    c^-1(Xthr) = acos(Xthi^(1 / alpha+1))
    p(phr) = 1 / 2π
    c(phr) = phr / 2π
    c^-1(Xphr) = 2* π * Xphr
 */

#endif // !RUSSIAN_ROULETTE_HPP