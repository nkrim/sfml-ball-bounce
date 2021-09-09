/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Bounce
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: ball.h
 *  Header file for ball (sfml-drawable) class.
 *  Represents a customizable circle object to be drawn by SFML applications.
 *  The ball will bounce on the boundaries of the window.
 * **************************************************************************** */

#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include <string>

const float BALL_DEFAULT_ELASTICITY = 0.90f;
const float BALL_DEFAULT_FRICTION = 0.05f;

class Ball : public sf::Drawable {
private:
    sf::CircleShape _circle;
    sf::Vector2f _velocity;
    float _elasticity;
    float _friction;

protected:
    // override sf::Drawable functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    Ball();
    Ball(float radius);
    Ball(     float         radius,
        const sf::Color&    color,
              float         border_size,
        const sf::Color&    border_color,
        const sf::Vector2f& position);

    // updates
    void bounce(sf::Vector2f window_size, sf::Vector2f accel, float t);

    // mutators
    void set_radius(float radius);
    void set_border_size(float border_size);
    void set_color(sf::Color color);
    void set_border_color(sf::Color border_color);
    void set_position(sf::Vector2f position);
    void set_velocity(sf::Vector2f velocity);
    void set_elasticity(float elasticity);
    void set_friction(float friction) const;

    // accessors
    float get_radius() const;
    float get_border_size() const;
    sf::Color get_color() const;
    sf::Color get_border_color() const;
    sf::Vector2f get_position() const;
    sf::Vector2f get_velocity() const;
    float get_elasticity() const;
    float get_friction() const;

    // extra accessors
    sf::FloatRect get_bounds() const;

    // debug
    std::string get_info() const;
};

#endif
