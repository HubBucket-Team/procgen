#include "Turtle.h"

namespace logo
{

    std::vector<sf::Vertex> compute_vertices(const Turtle& turtle, int n_iter)
    {
        Walk walk;
        walk.curr_pos   = turtle.starting_pos;
        walk.curr_angle = turtle.starting_angle;
        walk.vertices   = { walk.curr_pos };
        
        lsys::LSystem lsys = turtle.lsys;
        
        const auto res = lsys.iter(n_iter);

        for (auto c : res) {
            if (turtle.orders.count(c) > 0) {
                turtle.orders.at(c)(turtle, walk);
            }
            else {
                // Do nothing.
            }
        }

        return walk.vertices;
    }

    void go_forward(const Turtle& turtle, Walk& walk)
    {
        float dx = turtle.step * std::cos(walk.curr_angle);
        float dy = turtle.step * std::sin(walk.curr_angle);
        walk.curr_pos += {dx, dy};
        walk.vertices.push_back(walk.curr_pos);
    }
    void turn_right(const Turtle& turtle, Walk& walk)
    {
        walk.curr_angle += turtle.delta_angle;
    }

    void turn_left(const Turtle& turtle, Walk& walk)
    {
        walk.curr_angle -= turtle.delta_angle;
    }
}