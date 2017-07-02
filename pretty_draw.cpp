#include <iostream>

#include "pretty_draw.h"

DrawManager::DrawManager(sf::Time beg, const std::vector<sf::Vertex>& vec)
    : begin{beg}
    , vertices{vec}
{
}


void DrawManager::progressive_draw(sf::RenderTarget& target, sf::Time now)
{
    const int time_step = 10; // In ms

    // 1 11  : (11-1)/100   =  0->1
    // 1 250 : (250-1)/100  =  2->3
    unsigned int n_draw = (now.asMilliseconds() - begin.asMilliseconds()) / time_step;

    if (n_draw + 1 < vertices.size()) {
        std::vector<sf::Vertex> todraw (vertices.begin(), vertices.begin() + n_draw + 1);
        target.draw(todraw.data(), todraw.size(), sf::LineStrip);
    }
    else {
        target.draw(vertices.data(), vertices.size(), sf::LineStrip);
    }
}
