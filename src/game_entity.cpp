#include "../include/game_entity.h"

void GameEntity::update()
{
    sf::Vector2u tilePosition = sf::Vector2u(20, 20);
    entitySprite.setPosition(position);
    entitySprite.setTextureRect(rectSourceEntity);
}

sf::Vector2f GameEntity::getPosition()
{
    return position;
}

EntityDirection GameEntity::getDirection()
{
    return direction;
}

sf::Sprite GameEntity::getSprite()
{
    return entitySprite;
}

