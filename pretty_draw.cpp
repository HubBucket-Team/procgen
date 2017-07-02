#include <iostream>

#include "pretty_draw.h"

DrawManager::DrawManager(sf::Time beg, const std::vector<sf::Vertex>& vec)
    : begin{beg}
    , vertices{vec}
{
}


void DrawManager::progressive_draw(sf::RenderTarget& target, sf::Time now) const
{
    const int time_step = 5; // In ms

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

using std::sin;

void DrawManager::rainbowify()
{
    // https://krazydad.com/tutorials/makecolors.php
    float freq = .005;
    int j = 0;
    for (auto& vertex : vertices) {
        sf::Color col (sin(freq*j + 0)*127 + 128,
                       sin(freq*j + 2)*127 + 128,
                       sin(freq*j + 4)*127 + 128);
        vertex.color = col;
        ++j;
    }
}
