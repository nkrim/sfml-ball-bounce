/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Bounce
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: ball.cpp
 *  Definition file for ball (sfml-drawable) class.
 *  Represents a customizable circle object to be drawn by SFML applications.
 *  The ball will bounce on the boundaries of the window.
 * **************************************************************************** */

#include "ball.h"

#include <limits>
#include <cmath>
#include "utils.h"

// debug includes
#include <sstream>
#include <iostream>

// constructors
Ball::Ball()
    :_circle(0), _velocity{0,0}
    ,_elasticity(BALL_DEFAULT_ELASTICITY)
    ,_friction(BALL_DEFAULT_FRICTION)
{ }
Ball::Ball(float radius)
    :_circle(radius), _velocity{0,0}
    ,_elasticity(BALL_DEFAULT_ELASTICITY)
    ,_friction(BALL_DEFAULT_FRICTION)
{ }
Ball::Ball(
          float         radius,
    const sf::Color&    color,
          float         border_size,
    const sf::Color&    border_color,
    const sf::Vector2f& position
) 
    :_circle(radius), _velocity{0,0}
    ,_elasticity(BALL_DEFAULT_ELASTICITY)
    ,_friction(BALL_DEFAULT_FRICTION)
{
    _circle.setFillColor(color);
    _circle.setOutlineThickness(border_size);
    _circle.setOutlineColor(border_color);
    _circle.setPosition(position);
}

// override sf::Drawable functions
void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_circle);
}

// updates
void Ball::bounce(sf::Vector2f window_size, sf::Vector2f accel, float t) {
    // make adjustment to accel to stick ball to surface when numbers get small
    sf::FloatRect bounds = get_bounds();
    const float velocity_clamp = 5.0f;
    const float distance_clamp = 1.0f;
    const float distance_buffer = 0.1f;
    // stick left
    if(accel.x < 0 && fabs(bounds.left)<distance_clamp
        && _velocity.x <= 0 && fabs(_velocity.x)<velocity_clamp) 
    {
        _circle.setPosition(distance_buffer, bounds.top);
        _velocity.x = 0;
        _velocity.y -= _friction*t*_velocity.y;
        accel.x = 0;
    }
    // stick right
    if(accel.x > 0 && fabs(bounds.left+bounds.width-window_size.x)<distance_clamp
        && _velocity.x >= 0 && fabs(_velocity.x)<velocity_clamp) 
    {
        _circle.setPosition(window_size.x-bounds.width-distance_buffer, bounds.top);
        _velocity.x = 0;
        _velocity.y -= _friction*t*_velocity.y;
        accel.x = 0;
    }
    // stick top
    if(accel.y < 0 && fabs(bounds.top)<distance_clamp
        && _velocity.y <= 0 && fabs(_velocity.y)<velocity_clamp) 
    {
        _circle.setPosition(bounds.left, distance_buffer);
        _velocity.y = 0;
        _velocity.x -= _friction*t*_velocity.x;
        accel.y = 0;
    }
    // stick bottom
    if(accel.y > 0 && fabs(bounds.top+bounds.height-window_size.y)<distance_clamp
        && _velocity.y >= 0 && fabs(_velocity.y)<velocity_clamp) 
    {
        _circle.setPosition(bounds.left, window_size.y-(bounds.height+distance_buffer));
        _velocity.y = 0;
        _velocity.x -= _friction*t*_velocity.x;
        accel.y = 0;
    }

    // pre-compute known-constant quantities
    sf::Vector2f half_accel = 0.5f*accel;
    sf::FloatRect window_rect(0, 0, window_size.x, window_size.y);

    // loop to resolve multiple collisions in one frame
    do {
        // perform initial movement
        sf::Vector2f final_pos = _circle.getPosition() + t*_velocity + t*t*half_accel;

        // check for boundary breaks and adjust
        bounds = get_bounds();
        float half_width = 0.5f*bounds.width;
        float half_height = 0.5f*bounds.height;
        bool side_contains[4] = { // left then clockwise
            window_rect.contains(final_pos.x,              final_pos.y+half_height),
            window_rect.contains(final_pos.x+half_width,   final_pos.y),
            window_rect.contains(final_pos.x+bounds.width, final_pos.y+half_height),
            window_rect.contains(final_pos.x+half_width,   final_pos.y+bounds.height)
        };

        float max_float = std::numeric_limits<float>::max(); 
        float t_left_oob = max_float;
        float t_top_oob = max_float;
        float t_right_oob = max_float;
        float t_bottom_oob = max_float;

        // get shortest times for reaching out of bounds
        // left OoB
        if(!side_contains[0]) {
            if(accel.x != 0) {
                min_positive_quadratic_root(accel.x, _velocity.x, bounds.left, t_left_oob);
            }
            else {
                t_left_oob = (-bounds.left)/_velocity.x;
            }
        }
        // top OoB
        if(!side_contains[1]) {
            if(accel.y != 0) {
                min_positive_quadratic_root(accel.y, _velocity.y, bounds.top, t_top_oob);
            }
            else {
                t_top_oob = (-bounds.top)/_velocity.y;
            }
        }
        // right OoB
        if(!side_contains[2]) {
            if(accel.x != 0) {
                min_positive_quadratic_root(-accel.x, -_velocity.x, window_size.x-(bounds.left+bounds.width),  t_right_oob);
            }
            else {
                t_right_oob = (window_size.x-(bounds.left+bounds.width))/_velocity.x;
            }
        }
        // bottom OoB
        if(!side_contains[3]) {
            if(accel.y != 0)
                min_positive_quadratic_root(-accel.y, -_velocity.y, window_size.y-(bounds.top+bounds.height), t_bottom_oob);
            else {
                t_bottom_oob = (window_size.y-(bounds.top+bounds.height))/_velocity.y;
            }
        }

        // get minimum of all
        float t_min_oob = fmin(t_left_oob, fmin(t_top_oob, fmin(t_right_oob, t_bottom_oob)));
        sf::Vector2f bounce_normal {0,0};

        // no bounce, set new position and velocity, set t = 0 to exit loop
        if(t_min_oob == max_float) {
            _circle.setPosition(final_pos);
            _velocity += t*accel;
            t = 0;
        }

        // evaluate bounce
        else {
            // determine bounce normal from min_t, if a bounce even occurred
            if(t_min_oob == t_left_oob)
                bounce_normal += {1,0};
            if(t_min_oob == t_top_oob)
                bounce_normal += {0,1};
            if(t_min_oob == t_right_oob)
                bounce_normal += {-1,0};
            if(t_min_oob == t_bottom_oob)
                bounce_normal += {0,-1};
            // normalize
            normalize2f(bounce_normal);

            // set position of ball at t_min_oob
            _circle.setPosition(_circle.getPosition() + t_min_oob*_velocity + t_min_oob*t_min_oob*half_accel);

            // get velocity of ball at t_min_oob
            sf::Vector2f vel_t_min_oob = _velocity + t_min_oob*accel;
            // reflect velocity about bounce_normal
            _velocity = vel_t_min_oob - 2*dot2f(vel_t_min_oob, bounce_normal)*bounce_normal;
            _velocity *= _elasticity;

            // subtract t by the current timestep used for this bounce
            t -= t_min_oob;
        }

    // while there is still time to evaluate, do next move
    } while(t>0);


}

// mutators
void Ball::set_radius(float radius) {
    _circle.setRadius(radius);
}
void Ball::set_border_size(float border_size) {
    _circle.setOutlineThickness(border_size);
}
void Ball::set_color(sf::Color color) {
    _circle.setFillColor(color);
}
void Ball::set_border_color(sf::Color border_color) {
    _circle.setOutlineColor(border_color);
}
void Ball::set_position(sf::Vector2f position) {
    _circle.setPosition(position);
}
void Ball::set_velocity(sf::Vector2f velocity) {
    _velocity = velocity;
}
void Ball::set_elasticity(float elasticity) {
    _elasticity = elasticity;
}

// accessors
float Ball::get_radius() const {
    return _circle.getRadius();
}
float Ball::get_border_size() const {
    return _circle.getOutlineThickness();
}
sf::Color Ball::get_color() const {
    return _circle.getFillColor();
}
sf::Color Ball::get_border_color() const {
    return _circle.getOutlineColor();
}
sf::Vector2f Ball::get_position() const {
    return _circle.getPosition();
}
sf::Vector2f Ball::get_velocity() const {
    return _velocity;
}
float Ball::get_elasticity() const {
    return _elasticity;
}

// extra accessors
sf::FloatRect Ball::get_bounds() const {
    sf::Vector2f pos = _circle.getPosition();
    float two_r = 2*_circle.getRadius();
    return sf::FloatRect(pos.x, pos.y, two_r, two_r);
}

// debug
std::string Ball::get_info() const {
    std::stringstream info;
    info << "pos: (" << _circle.getPosition().x << ", " << _circle.getPosition().y << ")" << std::endl
         << "vel: (" << _velocity.x << ", " << _velocity.y << ")" << std::endl;
    return info.str();
}