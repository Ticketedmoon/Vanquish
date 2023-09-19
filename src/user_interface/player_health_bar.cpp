#include "user_interface/player_health_bar.h"

PlayerHealthBar::PlayerHealthBar(const std::shared_ptr<GameEntity>& entity, sf::Vector2f fillDimensions,
        sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
        float outlineThickness)
        : HealthBar(entity, fillDimensions, backgroundDimensions, position, fillColor, outlineColour, outlineThickness)
{

}

void PlayerHealthBar::update(GameState& gameState)
{
    sf::Vector2f newSize = sf::Vector2f(entity->getHealth(), healthBar.getSize().y);
    healthBar.setSize(newSize);
}

void PlayerHealthBar::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(healthBarBackground);
    renderTarget.draw(healthBar);
}
