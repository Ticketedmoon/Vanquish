#include "user_interface/enemy_health_bar.h"

EnemyHealthBar::EnemyHealthBar(const std::shared_ptr<GameEntity>& entity, sf::Vector2f fillDimensions,
        sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
        float outlineThickness)
        : HealthBar(entity, fillDimensions, backgroundDimensions, position, fillColor, outlineColour, outlineThickness)
{

}

void EnemyHealthBar::update(GameState& gameState)
{
    sf::Vector2f newSize = sf::Vector2f(entity->getHealth(), healthBar.getSize().y);
    healthBar.setSize(newSize);
    healthBarBackground.setPosition(sf::Vector2f(entity->getPosition().x - HEALTH_BAR_OFFSET_POSITION_X,
            entity->getPosition().y + HEALTH_BAR_OFFSET_POSITION_Y));
    healthBar.setPosition(sf::Vector2f(entity->getPosition().x - HEALTH_BAR_OFFSET_POSITION_X,
            entity->getPosition().y + HEALTH_BAR_OFFSET_POSITION_Y));
}

void EnemyHealthBar::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(healthBarBackground);
    renderTarget.draw(healthBar);
}
