#ifndef LSYSTEM_VIEW
#define LSYSTEM_VIEW


#include <cmath>

#include "DrawingParameters.h"
#include "LSystemBuffer.h"
#include "InterpretationMapBuffer.h"
 
constexpr int pow(int v, int e)
{
	return (e == 0) ? 1 : v * pow(v, e - 1);
}   

namespace procgui
{
    class LSystemView : public Observer<LSystem>,
                        public Observer<drawing::InterpretationMap>
    {
    public:
        LSystemView(std::shared_ptr<LSystem> lsys,
                    std::shared_ptr<drawing::InterpretationMap> map,
                    drawing::DrawingParameters param);

        //Getters
        drawing::DrawingParameters& get_parameters();
        LSystemBuffer& get_lsystem_buffer();
        InterpretationMapBuffer& get_interpretation_buffer();

        
        // Compute the vertices of the turtle interpretation of the LSystem.
        void compute_vertices();

        // Draw the vertices.
        void draw (sf::RenderTarget &target);
        
    private:
        void compute_bounding_box();
        static constexpr unsigned int n_subdivision = 3;
        void compute_subdivisions();
        void init_subdivisions();
        int find_index(const sf::Vertex& v) const;
        
        LSystemBuffer lsys_buff_;
        InterpretationMapBuffer interpretation_buff_;
        drawing::DrawingParameters params_;

        std::vector<sf::Vertex> vertices_;

        sf::FloatRect bounding_box_;
        std::array<sf::FloatRect, pow(4, n_subdivision)> boxes_;
    };
}

#endif