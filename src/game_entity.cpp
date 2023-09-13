#include "../include/game_entity.h"
#include "level.h"

GameEntity::GameEntity(uint8_t width, uint8_t height, float speed, sf::Vector2f position,
        sf::IntRect entitySpriteSheetDimRect, sf::Vector2u startingAnimationPosition, uint16_t health,
        sf::Vector2u spritePositionOffset)
        : entitySpriteSheetDimRect(entitySpriteSheetDimRect),
          speed(speed),
          startingAnimationPosition(startingAnimationPosition),
          spritePositionOffset(spritePositionOffset),
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

void GameEntity::setDirection(EntityDirection dir)
{
    this->direction = dir;
}

void GameEntity::updatePosition(GameClock& gameClock)
{
    if (isNextTileCollidable(gameClock))
    {
        return;
    }
    const float sign = direction == EntityDirection::UP || direction == EntityDirection::LEFT ? -1.0f : 1.0f;
    float xAccel = direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT ? (speed * sign) : 0;
    float yAccel = direction == EntityDirection::UP || direction == EntityDirection::DOWN ? (speed * sign) : 0;
    sf::Vector2f acceleration = sf::Vector2f(xAccel, yAccel);

    // update velocity through acceleration
    velocity += acceleration;

    // TODO REFACTOR ME CAN WE USE tilePosition?
    uint32_t worldWidth = (Level::getWorldWidth() - 1) * TILE_SIZE;
    uint32_t worldHeight = (Level::getWorldHeight() - 1) * TILE_SIZE;

    if (direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(gameClock.getDeltaTime(), worldWidth, getPosition().x, velocity.x);
        setPosition(nextCoordinatePair.coordinatePosition, getPosition().y);
        velocity.x = nextCoordinatePair.velocity;
    }

    if (direction == EntityDirection::UP || direction == EntityDirection::DOWN)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(gameClock.getDeltaTime(), worldHeight, getPosition().y, velocity.y);
        setPosition(getPosition().x, nextCoordinatePair.coordinatePosition);
        velocity.y = nextCoordinatePair.velocity;
    }
}

GameEntity::NextCoordinateVelocityPair GameEntity::getNextCoordinatePositionWithNextVelocity(const sf::Time deltaTime,
        uint32_t tileMapDimensionValue,
        float positionForCoordinate,
        float velocityForCoordinate)
{
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

bool GameEntity::isNextTileCollidable(GameClock& gameClock)
{
    // TODO Introduce a tile object rather than a pair here?
    sf::Vector2u nextTileFacingEntity = findNextTileDirection(gameClock.getDeltaTime());
    std::vector<std::vector<uint32_t>> world = Level::getWorld();
    return world.at(nextTileFacingEntity.y).at(nextTileFacingEntity.x) > 0;
}

sf::Vector2<uint32_t> GameEntity::findNextTileDirection(sf::Time deltaTime)
{
    sf::Vector2f position = getPosition();

    if (direction == EntityDirection::UP)
    {
        position.y += (velocity.y - speed) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::DOWN)
    {
        position.y += (velocity.y + speed) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::LEFT)
    {
        position.x += (velocity.x - speed) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::RIGHT)
    {
        position.x += (velocity.x + speed) * deltaTime.asSeconds();
    }

    float nextPlayerPosWithOffsetX = position.x + spritePositionOffset.x;
    float nextPlayerPosWithOffsetY = position.y + spritePositionOffset.y;
    uint32_t nextTileX = nextPlayerPosWithOffsetX > 0
            ? std::floor(nextPlayerPosWithOffsetX / TILE_SIZE)
            : 0;
    uint32_t nextTileY = nextPlayerPosWithOffsetY > 0
            ? std::floor(nextPlayerPosWithOffsetY / TILE_SIZE)
            : 0;
    return {nextTileX, nextTileY};
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
