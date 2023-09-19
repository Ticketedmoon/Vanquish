#include "../include/game_entity.h"
#include "level.h"

GameEntity::GameEntity(uint32_t id, uint8_t width, uint8_t height, float speed, sf::Vector2f position, uint16_t health,
        sf::Vector2u spritePositionOffset,
        std::unordered_map<std::string, std::shared_ptr<AnimationGroup>> animationGroup,
        uint16_t damage, float experiencePoints)
        : id(id),
          width(width),
          height(height),
          speed(speed),
          animationGroupMap(std::move(animationGroup)),
          health(health),
          damage(damage),
          totalExperiencePoints(experiencePoints),
          spritePositionOffset(spritePositionOffset)
{
    setPosition(position);
    spawnPosition = sf::Vector2f(position);
}

// TODO why are we using deltaTime to move animation - different per cpu?
void GameEntity::performAnimationByKey(GameClock& gameClock, const std::string& animationKey)
{
    std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(animationKey);
    animationGroup->currentAnimationTime += gameClock.getDeltaTime();
    if (animationGroup->currentAnimationTime >= animationGroup->animationCompletionTime)
    {
        AnimationManager::updateAnimationGroup(width, height, static_cast<int>(direction), animationGroup);
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
    sf::Vector2u nextTileFacingEntityDirection = findNextTileFromDirection(gameClock.getDeltaTime());

    if (nextTileFacingEntityDirection.x >= TileMap::getWorldWidthInTiles()
        || nextTileFacingEntityDirection.y >= TileMap::getWorldHeightInTiles())
    {
        return false;
    }

    Tile& tile = TileMap::getTile(nextTileFacingEntityDirection.x, nextTileFacingEntityDirection.y);

    return tile.getType() != Tile::Type::GRASS;
}

sf::Vector2u GameEntity::findNextTileFromDirection(const sf::Time deltaTime) const
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

float GameEntity::getTotalExperiencePoints() const
{
    return this->totalExperiencePoints / level;
}


// TODO make static constant?
float GameEntity::getTotalExperiencePointsRequiredForLevelUp()
{
    return 200;
}

void GameEntity::increaseLevel(float xpPointsDelta)
{
    std::cout << "Level Up!" << '\n';

    // Update level
     this->level += 1;

     // Update attributes
     damage += 5;

     // reset xp
     totalExperiencePoints = xpPointsDelta;
}

EntityDirection GameEntity::getDirection() const
{
    return direction;
}

void GameEntity::applyDamage(GameClock& gameClock, uint16_t damageAmount)
{
    uint16_t newHealth = health > damageAmount ? health - damageAmount : 0;
    this->health = newHealth;
}

void GameEntity::updateSpriteColour(sf::Color spriteColour)
{
    entitySprite.setColor(spriteColour);
}

bool GameEntity::isDead() const
{
    return this->health == 0;
}

uint8_t GameEntity::getLevel() const
{
    return level;
}

uint32_t GameEntity::getId() const
{
    return id;
}