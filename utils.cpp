/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Bounce
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: utils.cpp
 *  Definition file for utility functions.
 * **************************************************************************** */

#include "utils.h"

#include <cmath>

bool min_positive_quadratic_root(float a, float b, float c, float& root) {
    float b_squared = b*b;
    float four_a_c  = 4*a*c;
    if(four_a_c > b_squared)
        return false;

    float under_sqrt = sqrt(b_squared - four_a_c);
    float two_a = 2*a;
    float plus = (-b + under_sqrt)/(two_a);
    float minus = (-b - under_sqrt)/(two_a);

    if(plus < 0 && minus < 0)
        return false;
    if(plus < 0) {
        root = minus;
        return true;
    }
    if(minus < 0) {
        root = plus;
        return true;
    }
    root = fmin(plus, minus);
    return true;
}

float dot2f(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x*b.x + a.y*b.y;
}


float mag2f(const sf::Vector2f& v) {
    if(v.x == 0 && v.y == 0)
        return 0;
    return sqrt((float)(v.x*v.x + v.y*v.y));
}


sf::Vector2f& normalize2f(sf::Vector2f& v) {
    float m = mag2f(v);
    if(m == 0)
        return v;
    return v/=mag2f(v);
}