#include "../include/game_entity.h"

GameEntity::GameEntity(uint8_t width, uint8_t height, float speed, sf::Vector2f position, sf::IntRect entityDimRect,
                       sf::Vector2f startingAnimationPosition)
    : width(width),
      height(height),
      speed(speed),
      position(position),
      entityDimRect(entityDimRect),
      startingAnimationPosition(startingAnimationPosition)
{
}

sf::Vector2f GameEntity::getPosition() const
{
    return position;
}

EntityDirection GameEntity::getDirection() const
{
    return direction;
}

void GameEntity::setDirection(EntityDirection dir)
{
    this->direction = dir;
}

uint8_t GameEntity::getWidth() const
{
    return width;
}

uint8_t GameEntity::getHeight() const
{
    return height;
}

void GameEntity::reset()
{
    animationFrameStartTime = sf::Time::Zero;
    position = spawnPosition;
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
                getNextCoordinatePositionWithNextVelocity(deltaTime, tileMapWidth, position.x, velocity.x);
        position.x = nextCoordinatePair.coordinatePosition;
        velocity.x = nextCoordinatePair.velocity;
    }

    if (direction == EntityDirection::UP || direction == EntityDirection::DOWN)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(deltaTime, tileMapHeight, position.y, velocity.y);
        position.y = nextCoordinatePair.coordinatePosition;
        velocity.y = nextCoordinatePair.velocity;
    }
}

GameEntity::NextCoordinateVelocityPair GameEntity::getNextCoordinatePositionWithNextVelocity(const sf::Time &deltaTime,
                                                                                             uint32_t tileMapWidth,
                                                                                             float positionForCoordinate,
                                                                                             float velocityForCoordinate) {
    float positionDeltaX = positionForCoordinate + (velocityForCoordinate * deltaTime.asSeconds());
    if (positionDeltaX >= 0 && positionDeltaX <static_cast<float>(tileMapWidth))
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

uint8_t GameEntity::getSpriteSheetAnimationOffset(const EntityDirection dir) const
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

bool GameEntity::isColliding(const std::shared_ptr<GameEntity>& entityToCompare) const
{
    sf::Vector2f currGameEntityPos = this->getPosition();
    sf::Vector2f entityToComparePos = entityToCompare->getPosition();

    if ((currGameEntityPos.x + this->getWidth()) > entityToComparePos.x
        && currGameEntityPos.x < (entityToComparePos.x + entityToCompare->getWidth())
        && (currGameEntityPos.y + this->getHeight()) > entityToComparePos.y
        && currGameEntityPos.y < (entityToComparePos.y + entityToCompare->getHeight())) {
        return true;
    }

    return false;
}

void GameEntity::setSpeed(float speed)
{
    GameEntity::speed = speed;
}
