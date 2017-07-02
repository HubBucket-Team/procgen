#ifndef PRETTY_DRAW_H
#define PRETTY_DRAW_H


#include <vector>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


struct DrawManager
{
    DrawManager(sf::Time beg, const std::vector<sf::Vertex>& vec);
    
    void progressive_draw(sf::RenderTarget& target, sf::Time now);

    const sf::Time begin;
    std::vector<sf::Vertex> vertices;
};


#endif
