#include <cmath>
#include "procgui.h"
#include "LSystemView.h"

namespace procgui
{
    using namespace drawing;
    
    LSystemView::LSystemView(std::shared_ptr<LSystem> lsys,
                             std::shared_ptr<drawing::InterpretationMap> map,
                             drawing::DrawingParameters params)
        : Observer<LSystem> {lsys}
        , Observer<InterpretationMap> {map}
        , lsys_buff_ {lsys}
        , interpretation_buff_ {map}
        , params_ {params}
        , vertices_ {}
        , bounding_box_ {}
        , sub_boxes_ {}
        , is_selected_ {false}
    {
        // Invariant respected: cohesion between the LSystem/InterpretationMap
        // and the vertices. 
        Observer<LSystem>::add_callback([this](){compute_vertices();});
        Observer<InterpretationMap>::add_callback([this](){compute_vertices();});
        compute_vertices();
    }

    LSystemView::LSystemView(const LSystemView& other)
        : Observer<LSystem> {other.Observer<LSystem>::target_}
        , Observer<InterpretationMap> {other.Observer<InterpretationMap>::target_}
        , lsys_buff_ {other.lsys_buff_}
        , interpretation_buff_ {other.interpretation_buff_}
        , params_ {other.params_}
        , vertices_ {other.vertices_}
        , bounding_box_ {other.bounding_box_}
        , sub_boxes_ {other.sub_boxes_}
        , is_selected_ {other.is_selected_}
    {
        Observer<LSystem>::add_callback([this](){compute_vertices();});
        Observer<InterpretationMap>::add_callback([this](){compute_vertices();});
    }
    LSystemView& LSystemView::operator=(const LSystemView& other)
    {
        lsys_buff_ = other.lsys_buff_;
        interpretation_buff_ = other.interpretation_buff_;
        params_ = other.params_;
        vertices_ = other.vertices_;
        bounding_box_ = other.bounding_box_;
        sub_boxes_ = other.sub_boxes_;
        is_selected_ = other.is_selected_;

        Observer<LSystem>::add_callback([this](){compute_vertices();});
        Observer<InterpretationMap>::add_callback([this](){compute_vertices();});

        return *this;
    }


    drawing::DrawingParameters& LSystemView::get_parameters()
    {
        return params_;
    }
    LSystemBuffer& LSystemView::get_lsystem_buffer()
    {
        return lsys_buff_;
    }
    InterpretationMapBuffer& LSystemView::get_interpretation_buffer()
    {
        return interpretation_buff_;
    }

    
    void rainbowify(std::vector<sf::Vertex>& vertices)
    {
        // https://krazydad.com/tutorials/makecolors.php
        float freq = .005;
        int j = 0;
        for (auto& vertex : vertices) {
            sf::Color col (std::sin(freq*j + 0)*127 + 128,
                           std::sin(freq*j + 2)*127 + 128,
                           std::sin(freq*j + 4)*127 + 128);
            col.a = vertex.color.a;
            vertex.color = col;
            ++j;
        }
    }

    sf::Color value_between_two_colors(float value, const sf::Color& a, const sf::Color& b)
    {
        sf::Color col;
        col.r = (b.r - a.r) * value + a.r;
        col.g = (b.g - a.g) * value + a.g;
        col.b = (b.b - a.b) * value + a.b;
        return col;
    }

    sf::Color heat_map(float value, const std::vector<sf::Color>& colors)
    {
        // const int NUM_COLORS = 6
        // const std::array<sf::Color, NUM_COLORS> colors {{ {0,0,255}, {0,255,0}, {255,255,0}, {255,0,0} }}; true rainbow
        // const std::array<sf::Color, NUM_COLORS> colors {{ {0,0,255}, {155,0,255}, {255,0,255}, {255,255,0}, {255,155,0}, {255,0,0} }};
        value *= colors.size()-1;
        int i = std::floor(value);
        int j = i < colors.size()-1 ? i+1 : i;
        float frac_between = value - i;

        sf::Color col;
        col.r = (colors.at(i).r - colors.at(j).r)*frac_between + colors.at(i).r;
        col.g = (colors.at(i).g - colors.at(j).g)*frac_between + colors.at(i).g;
        col.b = (colors.at(i).b - colors.at(j).b)*frac_between + colors.at(i).b;
        return col;
    }
    
    void LSystemView::compute_vertices()
    {
        // Invariant respected: cohesion between the vertices and the bounding
        // boxes. 
        
        vertices_ = drawing::compute_vertices(*Observer<LSystem>::target_,
                                              *Observer<InterpretationMap>::target_,
                                              params_);
        bounding_box_ = geometry::compute_bounding_box(vertices_);
        sub_boxes_ = geometry::compute_sub_boxes(vertices_, MAX_SUB_BOXES);
        // rainbowify(vertices_);
        // for (size_t i = 0; i<vertices_.size(); ++i)
        // {
        //     // auto col = value_between_two_colors(static_cast<float>(i) / vertices_.size() ,
        //     //                                     sf::Color::Red, sf::Color::Blue);
        //     auto col = heat_map(static_cast<float>(i) / vertices_.size(), {sf::Color::Red, sf::Color::Blue, sf::Color::White});
        //     col.a = vertices_.at(i).color.a;
        //     vertices_.at(i).color = col;
        // }
        float top = bounding_box_.top;
        float bottom = top + bounding_box_.height;
        for (auto& v : vertices_)
        {
            auto col = heat_map((bottom - v.position.y) / (bottom - top), {sf::Color::Red, sf::Color::Blue// , sf::Color::White
                        });
            col.a = v.color.a;
            v.color = col;
        }
    }

    void LSystemView::draw(sf::RenderTarget &target)
    {
        // Interact with the models and re-compute the vertices if there is a
        // modification. 
        if (interact_with(*this, "", true, &is_selected_))
        {
            compute_vertices();
        }

        
        
        // Early out if there are no vertices.
        if (vertices_.size() == 0)
        {
            return;
        }

        // Draw the vertices.
        target.draw(vertices_.data(), vertices_.size(), sf::LineStrip);

        if (is_selected_)
        {
            // Draw the global bounding boxes.
            std::array<sf::Vertex, 5> box =
                {{ {{ bounding_box_.left, bounding_box_.top}},
                   {{ bounding_box_.left, bounding_box_.top + bounding_box_.height}},
                   {{ bounding_box_.left + bounding_box_.width, bounding_box_.top + bounding_box_.height}},
                   {{ bounding_box_.left + bounding_box_.width, bounding_box_.top}},
                   {{ bounding_box_.left, bounding_box_.top}}}};
            target.draw(box.data(), box.size(), sf::LineStrip);
        }

        // DEBUG
        // Draw the sub-bounding boxes.
        // for (const auto& box : sub_boxes_)
        // {
        //     std::array<sf::Vertex, 5> rect =
        //         {{ {{ box.left, box.top}, sf::Color(255,0,0,50)},
        //            {{ box.left, box.top + box.height}, sf::Color(255,0,0,50)},
        //            {{ box.left + box.width, box.top + box.height}, sf::Color(255,0,0,50)},
        //            {{ box.left + box.width, box.top}, sf::Color(255,0,0,50)}}};
        //     target.draw(rect.data(), rect.size(), sf::Quads);
        // }
    }

    bool LSystemView::select(const sf::Vector2f& click)
    {
        if (is_selected_)
        {
            return true;
        }
        for (const auto& rect : sub_boxes_)
        {
            if (rect.contains(sf::Vector2f(click)))
            {
                is_selected_ = true;
                return true;
            }
        }
        return false;
    }
}
