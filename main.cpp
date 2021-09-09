/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Bounce
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: main.cpp
 *  Test application for ball.h
 * **************************************************************************** */

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <regex>
#include <iostream>
#include <cstdint>

#include "ball.h"
#include "utils.h"

int main()
{
    const float SIZE = 1024.0f;
    const float R = 64.0f;
    const float g = 9.8f;

    sf::Vector2f gravity {0, -9.8};
    bool gravity_key_values[4] = {false,false,false,false};
    bool gravity_adjusted = false;

    sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "SFML works!");
    window.setFramerateLimit(60);

    Ball b(R, sf::Color(0x0000ffff), 2, sf::Color(0xffffffff), {50, 50});

    // title and exposition
    std::cout << std::endl
              << "SFML Ball Bounce" << std::endl
              << "----------------" << std::endl
              << "- adjust gravity by holding down arrow keys" << std::endl
              << "- program starts with downward gravity, " << std::endl
              << "    but defaults to zero-g once any adjustments are made" << std::endl
              << std::endl;

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // arrow gravity key adjustments
            else if (event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Left)
                    gravity_key_values[0] = true;
                else if(event.key.code == sf::Keyboard::Up)
                    gravity_key_values[1] = true;
                else if(event.key.code == sf::Keyboard::Right)
                    gravity_key_values[2] = true;
                else if(event.key.code == sf::Keyboard::Down)
                    gravity_key_values[3] = true;
            }
            else if (event.type == sf::Event::KeyReleased) {
                if(event.key.code == sf::Keyboard::Left)
                    gravity_key_values[0] = false;
                else if(event.key.code == sf::Keyboard::Up)
                    gravity_key_values[1] = false;
                else if(event.key.code == sf::Keyboard::Right)
                    gravity_key_values[2] = false;
                else if(event.key.code == sf::Keyboard::Down)
                    gravity_key_values[3] = false;
            }
        }

        // adjust gravity depending on keys
        gravity = {0,0};
        gravity.x -= gravity_key_values[0];
        gravity.y -= gravity_key_values[1];
        gravity.x += gravity_key_values[2];
        gravity.y += gravity_key_values[3];
        normalize2f(gravity);
        // default gravity going down until adjusted by user
        if(!gravity_adjusted && gravity.x == 0 && gravity.y == 0)
            gravity = {0,1};
        else
            gravity_adjusted = true;
        gravity *= g;

        // get time-point of frame
        float t = clock.getElapsedTime().asSeconds();
        clock.restart();

        // bounce the ball
        b.bounce({SIZE,SIZE}, gravity, 20*t);

        // clear window
        window.clear();

        // draw objects
        window.draw(b);

        // display 
        window.display();
    }

    return 0;
}