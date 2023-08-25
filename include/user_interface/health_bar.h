#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class HealthBar : public sf::Drawable, public sf::Transformable
{
    public:
        HealthBar(sf::Vector2f dimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
                  float outlineThickness);
        void update(uint16_t deductionAmount);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

    private:
        sf::RectangleShape healthBar;
        sf::RectangleShape healthBarBackground;

};