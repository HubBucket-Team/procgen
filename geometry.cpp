#include <gsl/gsl>
#include "geometry.h"

namespace geometry
{
    sf::FloatRect compute_bounding_box(const std::vector<sf::Vertex>& vertices)
    {
        if (vertices.size() == 0)
        {
            return { 0, 0, 0, 0 };
        }
        const auto& first = vertices.at(0);
        // warning: top is at low value.
        float top = first.position.y, down = first.position.y;
        float left = first.position.x, right = first.position.x;
        for (const auto& v : vertices)
        {
            if (v.position.y < top)
            {
                top = v.position.y;
            }
            else if (v.position.y > down)
            {
                down = v.position.y;
            }

            if (v.position.x > right)
            {
                right = v.position.x;
            }
            else if (v.position.x < left)
            {
                left = v.position.x;
            }
        }
        return {left, top, right - left, down - top};
    }
    
    std::vector<sf::FloatRect> compute_sub_boxes(const std::vector<sf::Vertex>& vertices,
                                                 int max_boxes, int min_vertices)
    {
        Expects(max_boxes > 1);
        Expects(min_vertices > 2);

        std::vector<sf::FloatRect> boxes;
        int vertices_per_box = vertices.size()  / (max_boxes-1);
        vertices_per_box = vertices_per_box < min_vertices ? min_vertices : vertices_per_box;
        
        int n = 0;
        std::vector<sf::Vertex> box_vertices;
        for (size_t i = 0; i<vertices.size(); ++i)
        {
            if (n == vertices_per_box)
            {
                n = 0;
                i -= 2;
                boxes.push_back(compute_bounding_box(box_vertices));
                box_vertices.clear();
            }
            
            box_vertices.push_back(vertices.at(i));
            ++n;

            if (i == vertices.size()-1)
            {
                boxes.push_back(compute_bounding_box(box_vertices));
                break;
            }
        }

        return boxes;
    }

}