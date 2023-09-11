#include "user_interface/health_bar.h"

HealthBar::HealthBar(std::shared_ptr<GameEntity>& entity, sf::Vector2f dimensions, sf::Vector2f position,
                     sf::Color fillColor, sf::Color outlineColour, float outlineThickness)
                     : entity(entity)
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

void HealthBar::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    sf::Vector2f newSize = sf::Vector2f(entity->getHealth(), healthBar.getSize().y);
    healthBar.setSize(newSize);
}

void HealthBar::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(healthBarBackground);
    renderTarget.draw(healthBar);
}
