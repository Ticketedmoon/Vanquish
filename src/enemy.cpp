#include "../include/enemy.h"

Enemy::Enemy(std::shared_ptr<TextureManager>& textureManager, std::shared_ptr<Player>& player,
        sf::Vector2f position, sf::Vector2u spriteSheetRectPosition)
        : GameEntity(ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_SPEED, position,
                     sf::IntRect(spriteSheetRectPosition.x, spriteSheetRectPosition.y, ENEMY_WIDTH, ENEMY_HEIGHT),
                     spriteSheetRectPosition, STARTING_ENEMY_HEALTH,
                     sf::Vector2u(std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT) * 0.5f,
                                  std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT))),
          m_player(player)
{
    sf::Texture& texture = *(textureManager->getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY));
    entitySprite = sf::Sprite(texture, entitySpriteSheetDimRect);
    entitySprite.scale(ENEMY_SCALE_FACTOR, ENEMY_SCALE_FACTOR);
    entitySprite.setPosition(getPosition());
    entitySprite.setTextureRect(entitySpriteSheetDimRect);

    // TEMPORARY, NICE VISUAL QUE BUT WHAT WE WANT LONG-TERM
    // TODO INSTEAD HIGHLIGHT THE GROUND OR NAMEPLATE OF THE ENEMY TO INDICATE POWER LEVEL (damage).
    if (damage >= 15)
    {
        entitySprite.setColor(sf::Color::Red);
    }
}

// TODO REFACTOR BELOW
void Enemy::update(GameState& gameState)
{
    const sf::Vector2f& position = getPosition();
    uint32_t tileUnderEnemyX = std::floor((position.x + static_cast<float>(spritePositionOffset.x)) / TILE_SIZE);
    uint32_t tileUnderEnemyY = std::floor((position.y + static_cast<float>(spritePositionOffset.y)) / TILE_SIZE);

    tilePosition = sf::Vector2u(tileUnderEnemyX, tileUnderEnemyY);
    entitySprite.setPosition(position);
    entitySprite.setTextureRect(entitySpriteSheetDimRect);

    GameClock& gameClock = gameState.getClock();
    uint64_t milliseconds = gameClock.getWorldTimeMs();

    if (milliseconds < entityWaitTimeBeforeMovementMs)
    {
        return;
    }

    if (isEnemyInProximityOfTarget(position, m_player->getPosition(), 24))
    {
        damagePlayer(gameClock);
        return;
    }

    if (isEnemyInProximityOfTarget(position, m_player->getPosition(), WANDER_DISTANCE))
    {
        moveToDestination(gameClock, m_player->getPosition());
        return;
    }

    if (!isEnemyInProximityOfTarget(position, spawnPosition, WANDER_DISTANCE))
    {
        // Return to spawn area if too far away.
        moveToDestination(gameClock, spawnPosition);
        return;
    }

    // Move randomly
    updateEntityToRandomDirection(gameClock, MAX_SPRITE_SHEET_FRAMES);
}

void Enemy::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(entitySprite);
}

void Enemy::damagePlayer(GameClock& gameClock)
{
    uint16_t playerHealth = m_player->getHealth();
    int timeNowSeconds = static_cast<int>(gameClock.getWorldTimeSeconds());
    bool hasEnemyAttackedAfterTimeWindow = timeNowSeconds - lastTimeEnemyAttacked >= 3;

    if (playerHealth > 0 && hasEnemyAttackedAfterTimeWindow) {
        uint16_t newHealth = playerHealth > damage ? playerHealth - damage : 0;
        m_player->setHealth(newHealth);
        lastTimeEnemyAttacked = timeNowSeconds;
    }
}

void Enemy::moveToDestination(GameClock& gameClock, sf::Vector2f destinationPoint)
{
    sf::Vector2f newPosition = getPosition();
    bool isEnemyToLeftOfDestination = newPosition.x < destinationPoint.x;
    bool isEnemyAboveDestination = newPosition.y < destinationPoint.y;

    bool isEnemyOutsideRangeOfDestinationOnYAxis =
            std::abs(newPosition.y - destinationPoint.y) > HORIZONTAL_DIRECTION_WINDOW_SIZE_FOR_ENEMY_ANIMATION;

    EntityDirection directionHorizontal = isEnemyToLeftOfDestination ? EntityDirection::RIGHT : EntityDirection::LEFT;
    EntityDirection directionVertical = isEnemyAboveDestination ? EntityDirection::DOWN : EntityDirection::UP;

    setDirection(directionHorizontal);
    updatePosition(gameClock);

    if (isEnemyOutsideRangeOfDestinationOnYAxis)
    {
        setDirection(directionVertical);
        updatePosition(gameClock);
    }

    performAnimation(gameClock, MAX_SPRITE_SHEET_FRAMES);
}

bool Enemy::isEnemyInProximityOfTarget(sf::Vector2f sourceLocation, sf::Vector2f targetLocation, uint32_t distance)
{
    return sqrt(pow(sourceLocation.x - targetLocation.x, 2) + pow(sourceLocation.y - targetLocation.y, 2)) < distance;
}

EntityType Enemy::getType()
{
    return EntityType::ENEMY;
}

sf::Time Enemy::getAnimationFrameDuration()
{
    return animationFrameDuration;
}
