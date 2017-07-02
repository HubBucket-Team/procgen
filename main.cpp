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
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Procgen");
    window.setVerticalSyncEnabled(true);
    
    LSystem lsys { "F", { { 'F', "G-F-G" }, { 'G', "F+G+F" } } };

    Turtle::lsys_interpretation intr { { 'F', go_forward },
                                       { 'G', go_forward },
                                       { '+', turn_left  },
                                       { '-', turn_right } };

    Turtle turtle { { 400, 100 }, 0, degree_to_rad(60), 5, lsys, intr };

    auto vertices = compute_vertices(turtle, 7);
    
    sf::Clock clock;
    DrawManager draw (clock.getElapsedTime(), vertices);
    while (window.isOpen())
    {
            sf::Event event;
            if (window.pollEvent(event))
            {
                    if (event.type == sf::Event::Closed ||
                        (event.type == sf::Event::KeyPressed &&
                         event.key.code == sf::Keyboard::Escape))
                    {
                            window.close();
                    }
            }

            window.clear();
            // window.draw(vertices.data(), vertices.size(), sf::LineStrip);
            draw.progressive_draw(window, clock.getElapsedTime());
            window.display();
    }
    
    return 0;
}
