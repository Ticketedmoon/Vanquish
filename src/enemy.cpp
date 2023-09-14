#include "../include/enemy.h"

Enemy::Enemy(std::shared_ptr<TextureManager>& textureManager, std::shared_ptr<Player>& player,
        sf::Vector2f position, sf::Vector2u spriteSheetRectPosition)
        : GameEntity(ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_SPEED, position,
                     sf::IntRect(spriteSheetRectPosition.x, spriteSheetRectPosition.y, ENEMY_WIDTH, ENEMY_HEIGHT),
                     spriteSheetRectPosition, STARTING_ENEMY_HEALTH,
                     sf::Vector2u(std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT) * 0.5f,
                             std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT))),
          player(player)
{
    sf::Texture& texture = *(textureManager->getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY));
    entitySprite = sf::Sprite(texture, entitySpriteSheetDimRect);
    entitySprite.scale(ENEMY_SCALE_FACTOR, ENEMY_SCALE_FACTOR);
    entitySprite.setPosition(getPosition());
    entitySprite.setTextureRect(entitySpriteSheetDimRect);

    // TEMPORARY, NICE VISUAL QUE BUT WHAT WE WANT LONG-TERM
    // INSTEAD HIGHLIGHT THE GROUND OR NAMEPLATE OF THE ENEMY TO INDICATE POWER LEVEL (damage).
    if (damage >= 15)
    {
        entitySprite.setColor(sf::Color::Red);
    }
}

void Enemy::update(GameState& gameState)
{
    const sf::Vector2f& position = getPosition();
    uint32_t tileUnderEnemyX = floor((position.x + spritePositionOffset.x) / TILE_SIZE);
    uint32_t tileUnderEnemyY = floor((position.y + spritePositionOffset.y) / TILE_SIZE);

    tilePosition = sf::Vector2u(tileUnderEnemyX, tileUnderEnemyY);
    entitySprite.setPosition(position);
    entitySprite.setTextureRect(entitySpriteSheetDimRect);

    GameClock& gameClock = gameState.getClock();

    // TODO REFACTOR BELOW
    int milliseconds = gameClock.getWorldTimeMs();
    if (milliseconds > entityWaitTimeBeforeMovement)
    {
        if (isEnemyInProximityOfTarget(position.x, position.y, player->getPosition().x, player->getPosition().y,
                                       WANDER_DISTANCE))
        {
            moveToDestination(gameClock, player->getPosition().x, player->getPosition().y);
            if (isEnemyInProximityOfTarget(position.x, position.y, player->getPosition().x, player->getPosition().y,
                                           24))
            {
                damagePlayer(gameClock.getWorldTimeSeconds());
            }
        }
        else
        {
            if (isEnemyInProximityOfTarget(position.x, position.y, spawnPosition.x, spawnPosition.y, WANDER_DISTANCE))
            {
                // Move randomly
                updatePosition(gameClock);
                if (milliseconds > (entityWaitTimeBeforeMovement + 250))
                {
                    entityWaitTimeBeforeMovement = std::experimental::randint(milliseconds + MIN_ENEMY_MOVE_RATE,
                                                                              milliseconds + MAX_ENEMY_MOVE_RATE);
                    updateEntityToRandomDirection();
                }
            }
            else
            {
                // Return to spawn area if too far away.
                moveToDestination(gameClock, spawnPosition.x, spawnPosition.y);
            }
        }
    }

    uint32_t spriteSheetTop = startingAnimationPosition.y +
                              (ENEMY_HEIGHT * getSpriteSheetAnimationOffset(direction));
    uint32_t spriteSheetLeft = entitySpriteSheetDimRect.left == (startingAnimationPosition.x + (ENEMY_WIDTH * 2))
            ? startingAnimationPosition.x
            : entitySpriteSheetDimRect.left + ENEMY_WIDTH;
    updateAnimation(gameClock.getDeltaTime(), spriteSheetTop, spriteSheetLeft);
}

void Enemy::damagePlayer(const float worldTimeSeconds)
{
    uint16_t playerHealth = player->getHealth();
    int timeNowSeconds = static_cast<int>(worldTimeSeconds);
    bool hasEnemyAttackedAfterTimeWindow = timeNowSeconds - lastTimeEnemyAttacked >= 3;

    if (playerHealth > 0 && hasEnemyAttackedAfterTimeWindow) {
        // TODO RESEARCH IF WE NEED DELTA TIME SINCE WE ARE ALREADY USING WORLD CLOCK
        uint16_t newHealth = playerHealth > damage ? playerHealth - damage : 0;
        player->setHealth(newHealth);
        lastTimeEnemyAttacked = timeNowSeconds;
    }
}

void Enemy::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(entitySprite);
}

void Enemy::moveToDestination(GameClock& gameClock, float destinationX, float destinationY) {

    sf::Vector2f newPosition = getPosition();
    bool isEnemyToLeftOfDestination = newPosition.x < destinationX;
    bool isEnemyAboveDestination = newPosition.y < destinationY;

    bool isEnemyOutsideRangeOfPlayerOnYAxis =
            abs(newPosition.y - player->getPosition().y) > HORIZONTAL_DIRECTION_WINDOW_SIZE_FOR_ENEMY_ANIMATION;

    EntityDirection directionHorizontal = isEnemyToLeftOfDestination ? EntityDirection::RIGHT : EntityDirection::LEFT;
    EntityDirection directionVertical = isEnemyAboveDestination ? EntityDirection::DOWN : EntityDirection::UP;

    setDirection(directionHorizontal);
    updatePosition(gameClock);

    if (isEnemyOutsideRangeOfPlayerOnYAxis)
    {
        setDirection(directionVertical);
        updatePosition(gameClock);
    }
}

bool Enemy::isEnemyInProximityOfTarget(float sourceLocationX, float sourceLocationY, float targetLocationX,
                                       float targetLocationY, uint32_t distance)
{
    return sqrt(pow(sourceLocationX - targetLocationX, 2) + pow(sourceLocationY - targetLocationY, 2)) < distance;
}

EntityType Enemy::getType()
{
    return EntityType::ENEMY;
}

sf::Time Enemy::getAnimationFrameDuration()
{
    return animationFrameDuration;
}
