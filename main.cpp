#include <iostream>

#include<SFML/Graphics.hpp>

#include "LSystem.h"
#include "Turtle.h"
#include "math.h"
#include "pretty_draw.h"

using namespace lsys;
using namespace logo;
using namespace math;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 700), "Procgen");
    window.setVerticalSyncEnabled(true);
    
    LSystem lsys { "F", { { 'F', "G-F-G" }, { 'G', "F+G+F" } } };

    Turtle::lsys_interpretation intr { { 'F', go_forward },
                                       { 'G', go_forward },
                                       { '+', turn_left  },
                                       { '-', turn_right } };

    Turtle turtle { { 785, 680 }, degree_to_rad(240), degree_to_rad(60), 3, lsys, intr };

    auto vertices = compute_vertices(turtle, 8);
    
    sf::Clock clock;
    auto curr_time = clock.getElapsedTime();
    DrawManager draw (curr_time, vertices);
    draw.rainbowify();
    bool wait = true;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                 event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed &&
                     event.key.code == sf::Keyboard::Space &&
                     wait ) {
                wait = false;
            }
        }

        if (wait) {
            curr_time = clock.getElapsedTime();
        }
        else {
            window.clear();
//            window.draw(vertices.data(), vertices.size(), sf::LineStrip);
            draw.progressive_draw(window, clock.getElapsedTime() - curr_time);
            window.display();
        }
    }
    
    return 0;
}
