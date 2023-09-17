#include "../include/enemy.h"

Enemy::Enemy(std::shared_ptr<TextureManager>& textureManager, std::shared_ptr<Player>& player,
        sf::Vector2f position, sf::Vector2u spriteSheetRectPosition)
        : GameEntity(ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_SPEED, position, STARTING_ENEMY_HEALTH,
        sf::Vector2u(std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT) * 0.5f,
                std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT)),
        {
                {
                        HUMAN_CHARACTER_SPRITE_SHEET_A_KEY,
                        std::make_shared<AnimationGroup>(3, spriteSheetRectPosition, sf::seconds(1.f / 6.f),
                                sf::IntRect(spriteSheetRectPosition.x, spriteSheetRectPosition.y, ENEMY_WIDTH,
                                        ENEMY_HEIGHT))
                }
        }),
        m_player(player)
{
    sf::Texture& texture = *(textureManager->getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY));
    std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY);

    entitySprite = sf::Sprite(texture, animationGroup->entitySpriteSheetDimRect);
    entitySprite.scale(ENEMY_SCALE_FACTOR, ENEMY_SCALE_FACTOR);
    entitySprite.setPosition(getPosition());
    entitySprite.setTextureRect(animationGroup->entitySpriteSheetDimRect);

    // TEMPORARY, NICE VISUAL QUE BUT WHAT WE WANT LONG-TERM
    // TODO INSTEAD HIGHLIGHT THE GROUND OR NAMEPLATE OF THE ENEMY TO INDICATE POWER LEVEL (damage).
    if (damage >= 15)
    {
        entitySprite.setColor(sf::Color::Red);
    }
}

void Enemy::update(GameState& gameState)
{
    const sf::Vector2f& position = getPosition();
    uint32_t tileUnderEnemyX = std::floor((position.x + static_cast<float>(spritePositionOffset.x)) / TILE_SIZE);
    uint32_t tileUnderEnemyY = std::floor((position.y + static_cast<float>(spritePositionOffset.y)) / TILE_SIZE);

    std::shared_ptr<AnimationGroup>& animationGroup = animationGroupMap.at(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY);
    tilePosition = sf::Vector2u(tileUnderEnemyX, tileUnderEnemyY);
    entitySprite.setPosition(position);
    entitySprite.setTextureRect(animationGroup->entitySpriteSheetDimRect);

    GameClock& gameClock = gameState.getClock();
    uint64_t milliseconds = gameClock.getWorldTimeMs();

    if (milliseconds < waitTimeBeforeMovementMs)
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
        // Chase player
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
    updateEntityToRandomDirection(gameClock, HUMAN_CHARACTER_SPRITE_SHEET_A_KEY);
}

void Enemy::damagePlayer(GameClock& gameClock)
{
    int timeNowSeconds = static_cast<int>(gameClock.getWorldTimeSeconds());
    bool hasEnemyAttackedAfterTimeWindow = timeNowSeconds - lastPlayerAttackSeconds >= 3;
    if (hasEnemyAttackedAfterTimeWindow)
    {
        m_player->takeDamage(gameClock, damage);
        lastPlayerAttackSeconds = timeNowSeconds;
    }
}

void Enemy::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(entitySprite);
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

    performAnimationByKey(gameClock, HUMAN_CHARACTER_SPRITE_SHEET_A_KEY, false);
}

bool Enemy::isEnemyInProximityOfTarget(sf::Vector2f sourceLocation, sf::Vector2f targetLocation, uint32_t distance)
{
    return sqrt(pow(sourceLocation.x - targetLocation.x, 2) + pow(sourceLocation.y - targetLocation.y, 2)) < distance;
}

EntityType Enemy::getType()
{
    return EntityType::ENEMY;
}