#include "../include/game_entity.h"

sf::Vector2f GameEntity::getPosition()
{
    return position;
}

EntityDirection GameEntity::getDirection()
{
    return direction;
}

void GameEntity::setDirection(EntityDirection dir)
{
    this->direction = dir;
}

void GameEntity::reset()
{
    animationFrameStartTime = sf::Time::Zero;
    position = spawnPosition;
}