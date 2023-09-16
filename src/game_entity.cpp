#include "../include/game_entity.h"
#include "level.h"

GameEntity::GameEntity(uint8_t width, uint8_t height, float speed, sf::Vector2f position,
        sf::IntRect entitySpriteSheetDimRect, sf::Vector2u startingAnimationPosition, uint16_t health,
        sf::Vector2u spritePositionOffset)
        : width(width),
          height(height),
          speed(speed),
          health(health),
          entitySpriteSheetDimRect(entitySpriteSheetDimRect),
          spritePositionOffset(spritePositionOffset),
          startingAnimationPosition(startingAnimationPosition)
{
    setPosition(position);
    spawnPosition = sf::Vector2f(position);
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

    // TODO REFACTOR ME CAN WE USE tiles everywhere over world coordinates?

    if (direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(gameClock.getDeltaTime(), TileMap::getWorldWidthInTiles() - 1,
                        getPosition().x, velocity.x);

        setPosition(nextCoordinatePair.coordinatePosition, getPosition().y);
        velocity.x = nextCoordinatePair.velocity;
    }

    if (direction == EntityDirection::UP || direction == EntityDirection::DOWN)
    {
        NextCoordinateVelocityPair nextCoordinatePair =
                getNextCoordinatePositionWithNextVelocity(gameClock.getDeltaTime(),
                        TileMap::getWorldHeightInTiles() - 1, getPosition().y, velocity.y);

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
    auto mapLimit = static_cast<float>(tileMapDimensionValue * TILE_SIZE);

    if (positionDeltaForCoordinate >= 0 && positionDeltaForCoordinate < mapLimit)
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

bool GameEntity::isNextTileCollidable(GameClock& gameClock) const
{
    sf::Vector2u nextTileFacingEntityDirection = findNextTileDirection(gameClock.getDeltaTime());
    if (nextTileFacingEntityDirection.x >= TileMap::getWorldWidthInTiles()
        || nextTileFacingEntityDirection.y >= TileMap::getWorldHeightInTiles())
    {
        return false;
    }

    Tile& tile = TileMap::getTile(nextTileFacingEntityDirection.x, nextTileFacingEntityDirection.y);

    return tile.getValue() > 0;
}

sf::Vector2u GameEntity::findNextTileDirection(const sf::Time deltaTime) const
{
    sf::Vector2f nextPosition = getPosition();

    if (direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT)
    {
        float sign = direction == EntityDirection::LEFT ? -1.0 : 1.0;
        nextPosition.x += (velocity.x + (speed * sign)) * deltaTime.asSeconds();
    }

    if (direction == EntityDirection::UP || direction == EntityDirection::DOWN)
    {
        float sign = direction == EntityDirection::UP ? -1.0 : 1.0;
        nextPosition.y += (velocity.y + (speed * sign)) * deltaTime.asSeconds();
    }

    float nextPlayerPosWithOffsetX = nextPosition.x + spritePositionOffset.x;
    float nextPlayerPosWithOffsetY = nextPosition.y + spritePositionOffset.y;
    uint32_t nextTileX = nextPlayerPosWithOffsetX > 0
            ? std::floor(nextPlayerPosWithOffsetX / TILE_SIZE)
            : 0;
    uint32_t nextTileY = nextPlayerPosWithOffsetY > 0
            ? std::floor(nextPlayerPosWithOffsetY / TILE_SIZE)
            : 0;

    return {nextTileX, nextTileY};
}

void GameEntity::updateEntityToRandomDirection(GameClock& gameClock, uint8_t maxSpriteSheetFrames)
{
    uint64_t milliseconds = gameClock.getWorldTimeMs();
    if (milliseconds > (entityWaitTimeBeforeMovementMs + 250))
    {
        entityWaitTimeBeforeMovementMs = std::experimental::randint(milliseconds + MIN_ENTITY_MOVE_RATE_MS,
                milliseconds + MAX_ENTITY_MOVE_RATE_MS);
        int directionIndex = std::experimental::randint(0, 3);
        auto randomDirection = static_cast<EntityDirection>(directionIndex);
        this->setDirection(randomDirection);
    }

    updatePosition(gameClock);
    performAnimation(gameClock, maxSpriteSheetFrames);
}

void GameEntity::performAnimation(GameClock& gameClock, uint8_t maxSpriteSheetFrames)
{
    int directionIndex = static_cast<int>(direction);
    uint32_t spriteSheetTop = startingAnimationPosition.y + (height * directionIndex);
    uint32_t spriteSheetLeft = entitySpriteSheetDimRect.left == startingAnimationPosition.x + (width * (maxSpriteSheetFrames - 1))
            ? startingAnimationPosition.x
            : entitySpriteSheetDimRect.left + width;
    updateAnimation(gameClock.getDeltaTime(), spriteSheetTop, spriteSheetLeft);
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
