#include "user_interface/health_bar.h"

HealthBar::HealthBar(sf::Vector2f dimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
                     float outlineThickness)
{
    healthBarBackground = sf::RectangleShape(dimensions);
    healthBarBackground.setPosition(position);
    healthBarBackground.setFillColor(sf::Color::Black);
    healthBarBackground.setOutlineColor(outlineColour);
    healthBarBackground.setOutlineThickness(outlineThickness);

    healthBar = sf::RectangleShape(dimensions);
    healthBar.setPosition(position);
    healthBar.setFillColor(fillColor);
    healthBar.setOutlineColor(outlineColour);
    healthBar.setOutlineThickness(outlineThickness);
}

void HealthBar::update(uint16_t healthValue)
{
    sf::Vector2f newSize = sf::Vector2f(healthValue, healthBar.getSize().y);
    healthBar.setSize(newSize);
}

void HealthBar::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(healthBarBackground);
    renderTarget.draw(healthBar);
}
