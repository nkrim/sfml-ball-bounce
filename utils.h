/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Bounce
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: utils.h
 *  Header file for utility functions.
 * **************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include <SFML/System.hpp>

bool min_positive_quadratic_root(float a, float b, float c, float& root);
float dot2f(const sf::Vector2f& a, const sf::Vector2f& b);
float mag2f(const sf::Vector2f& v);
sf::Vector2f& normalize2f(sf::Vector2f& v);

#endif
