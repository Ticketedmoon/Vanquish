#include "../include/game_entity.h"

sf::Vector2f GameEntity::getPosition()
{
    return position;
}

EntityDirection GameEntity::getDirection()
{
    return direction;
}

sf::Sprite& GameEntity::getSprite()
{
    return entitySprite;
}

void GameEntity::setDirection(EntityDirection dir) {
    this->direction = dir;
}