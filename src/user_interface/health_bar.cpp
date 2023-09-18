#include "user_interface/health_bar.h"

HealthBar::HealthBar(std::shared_ptr<GameEntity>& entity, sf::Vector2f fillDimensions,
        sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
        float outlineThickness)
        : entity(entity)
{
    healthBarBackground = sf::RectangleShape(backgroundDimensions);
    healthBarBackground.setPosition(position);
    healthBarBackground.setFillColor(sf::Color::Black);
    healthBarBackground.setOutlineColor(outlineColour);
    healthBarBackground.setOutlineThickness(outlineThickness);

    healthBar = sf::RectangleShape(fillDimensions);
    healthBar.setPosition(position);
    healthBar.setFillColor(fillColor);
    healthBar.setOutlineColor(outlineColour);
    healthBar.setOutlineThickness(outlineThickness);
}

void HealthBar::update(GameState& gameState)
{
    sf::Vector2f newSize = sf::Vector2f(entity->getHealth(), healthBar.getSize().y);
    healthBar.setSize(newSize);
}

void HealthBar::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(healthBarBackground);
    renderTarget.draw(healthBar);
}
