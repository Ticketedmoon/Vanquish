#include "user_interface/enemy_health_bar.h"

EnemyHealthBar::EnemyHealthBar(const std::shared_ptr<GameEntity>& entity, sf::Vector2f fillDimensions,
        sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
        float outlineThickness)
        : HealthBar(entity, fillDimensions, backgroundDimensions, position, fillColor, outlineColour, outlineThickness)
{

}

void EnemyHealthBar::update(GameState& gameState)
{
    sf::Vector2f healthBarDimensions = sf::Vector2f(entity->getHealth(), healthBar.getSize().y);
    healthBar.setSize(healthBarDimensions);
    
    float healthBarPositionX = (entity->getPosition().x + entity->getWidth()/2) - healthBarDimensions.x / 2;
    float healthBarPositionY = (entity->getPosition().y + entity->getHeight()) + 5;

    std::cout << healthBarPositionX << ", " << entity->getPosition().x << '\n';
    healthBarBackground.setPosition(sf::Vector2f(healthBarPositionX, healthBarPositionY));
    healthBar.setPosition(sf::Vector2f(healthBarPositionX, healthBarPositionY));
}

void EnemyHealthBar::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(healthBarBackground);
    renderTarget.draw(healthBar);
}

std::shared_ptr<GameEntity> EnemyHealthBar::getEntity() const
{
    return entity;
}
