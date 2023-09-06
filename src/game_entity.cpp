#include "../include/game_entity.h"

GameEntity::GameEntity(uint8_t width, uint8_t height, float speed, sf::Vector2f position,
                       sf::IntRect entitySpriteSheetDimRect, sf::Vector2f startingAnimationPosition)
    : width(width),
      height(height),
      speed(speed),
      entitySpriteSheetDimRect(entitySpriteSheetDimRect),
      startingAnimationPosition(startingAnimationPosition)
{
    setPosition(position);
    spawnPosition = sf::Vector2f(position);
}

void GameEntity::updateAnimation(sf::Time& deltaTime, uint32_t spriteSheetTop, uint32_t spriteSheetLeft)
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

void GameEntity::updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    const float sign = direction == EntityDirection::UP || direction == EntityDirection::LEFT ? -1.0f : 1.0f;
    float xAccel = direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT ? (speed * sign) : 0;
    float yAccel = direction == EntityDirection::UP || direction == EntityDirection::DOWN ? (speed * sign) : 0;
    sf::Vector2f acceleration = sf::Vector2f(xAccel, yAccel);

    // update velocity through acceleration
    velocity += acceleration;

    // TODO REFACTOR ME CAN WE USE tilePosition?
    uint32_t tileMapWidth = ((levelWidth-1) * width);
    uint32_t tileMapHeight = ((levelHeight-1) * height);

    if (direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(deltaTime, tileMapWidth, getPosition().x, velocity.x);
        setPosition(nextCoordinatePair.coordinatePosition, getPosition().y);
        velocity.x = nextCoordinatePair.velocity;
    }

    if (direction == EntityDirection::UP || direction == EntityDirection::DOWN)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(deltaTime, tileMapHeight, getPosition().y, velocity.y);
        setPosition(getPosition().x, nextCoordinatePair.coordinatePosition);
        velocity.y = nextCoordinatePair.velocity;
    }
}

GameEntity::NextCoordinateVelocityPair GameEntity::getNextCoordinatePositionWithNextVelocity(const sf::Time &deltaTime,
                                                                                             uint32_t tileMapDimensionValue,
                                                                                             float positionForCoordinate,
                                                                                             float velocityForCoordinate) {
    float positionDeltaX = positionForCoordinate + (velocityForCoordinate * deltaTime.asSeconds());
    if (positionDeltaX >= 0 && positionDeltaX <static_cast<float>(tileMapDimensionValue))
    {
        positionForCoordinate = positionDeltaX;
        velocityForCoordinate *= 0.5f;
    }
    else
    {
        positionForCoordinate = 0;
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

    if (dir == EntityDirection::LEFT)
    {
        return 1;
    }

    if (dir == EntityDirection::RIGHT)
    {
        return 2;
    }

    if (dir == EntityDirection::UP)
    {
        return 3;
    }
}

sf::FloatRect GameEntity::getSpriteGlobalBounds()
{
    return entitySprite.getGlobalBounds();
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
