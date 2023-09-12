#include "../include/game_entity.h"

GameEntity::GameEntity(uint8_t width, uint8_t height, float speed, sf::Vector2f position,
                       sf::IntRect entitySpriteSheetDimRect, sf::Vector2u startingAnimationPosition, uint16_t health)
    : entitySpriteSheetDimRect(entitySpriteSheetDimRect),
      speed(speed),
      startingAnimationPosition(startingAnimationPosition),
      health(health),
      width(width),
      height(height)
{
    setPosition(position);
    spawnPosition = sf::Vector2u(position);
}

void GameEntity::updateAnimation(sf::Time deltaTime, uint32_t spriteSheetTop, uint32_t spriteSheetLeft)
{
    animationFrameStartTime += deltaTime;
    if (animationFrameStartTime >= getAnimationFrameDuration())
    {
        entitySpriteSheetDimRect.top = spriteSheetTop;
        entitySpriteSheetDimRect.left = spriteSheetLeft;
        animationFrameStartTime = sf::Time::Zero;
    }
}

EntityDirection GameEntity::getDirection() const
{
    return direction;
}

void GameEntity::setDirection(EntityDirection dir)
{
    this->direction = dir;
}

void GameEntity::updatePosition(sf::Time deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    const float sign = direction == EntityDirection::UP || direction == EntityDirection::LEFT ? -1.0f : 1.0f;
    float xAccel = direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT ? (speed * sign) : 0;
    float yAccel = direction == EntityDirection::UP || direction == EntityDirection::DOWN ? (speed * sign) : 0;
    sf::Vector2f acceleration = sf::Vector2f(xAccel, yAccel);

    // update velocity through acceleration
    velocity += acceleration;

    // TODO REFACTOR ME CAN WE USE tilePosition?
    uint32_t worldWidth = (levelWidth - 1) * TILE_SIZE;
    uint32_t worldHeight = (levelHeight - 1) * TILE_SIZE;

    if (direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(deltaTime, worldWidth, getPosition().x, velocity.x);
        setPosition(nextCoordinatePair.coordinatePosition, getPosition().y);
        velocity.x = nextCoordinatePair.velocity;
    }

    if (direction == EntityDirection::UP || direction == EntityDirection::DOWN)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(deltaTime, worldHeight, getPosition().y, velocity.y);
        setPosition(getPosition().x, nextCoordinatePair.coordinatePosition);
        velocity.y = nextCoordinatePair.velocity;
    }
}

GameEntity::NextCoordinateVelocityPair GameEntity::getNextCoordinatePositionWithNextVelocity(const sf::Time deltaTime,
                                                                                             uint32_t tileMapDimensionValue,
                                                                                             float positionForCoordinate,
                                                                                             float velocityForCoordinate) {
    float positionDeltaForCoordinate = positionForCoordinate + (velocityForCoordinate * deltaTime.asSeconds());
    if (positionDeltaForCoordinate >= 0 && positionDeltaForCoordinate < static_cast<float>(tileMapDimensionValue))
    {
        positionForCoordinate = positionDeltaForCoordinate;
        velocityForCoordinate *= 0.5f;
    }
    else
    {
        velocityForCoordinate = 0.0f;
    }

    return NextCoordinateVelocityPair(positionForCoordinate, velocityForCoordinate);
}

// TODO REFACTOR
uint8_t GameEntity::getSpriteSheetAnimationOffset(const EntityDirection dir)
{
    if (dir == EntityDirection::DOWN)
    {
        return 0;
    }
    else if (dir == EntityDirection::LEFT)
    {
        return 1;
    }
    else if (dir == EntityDirection::RIGHT)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

void GameEntity::updateEntityToRandomDirection()
{
    int directionIndex = std::experimental::randint(0, 3);
    EntityDirection newDir = directionIndex == 0
            ? EntityDirection::UP : directionIndex == 1
            ? EntityDirection::RIGHT : directionIndex == 2
            ? EntityDirection::DOWN : EntityDirection::LEFT;

    this->setDirection(newDir);
}

uint8_t GameEntity::getWidth() const
{
    return width;
}

uint8_t GameEntity::getHeight() const
{
    return height;
}

uint16_t GameEntity::getHealth() const
{
    return this->health;
}

void GameEntity::setHealth(uint16_t newHealth)
{
    this->health = newHealth;
}

bool GameEntity::isDead() const
{
    return this->health == 0;
}
