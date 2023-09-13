#include "../include/enemy.h"

Enemy::Enemy(std::shared_ptr<TextureManager>& textureManager, std::shared_ptr<Player>& player,
             sf::Vector2f position,
             sf::Vector2u spriteSheetRectPosition,
             sf::Vector2u levelDimensions)
    : GameEntity(ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_SPEED, position,
                 sf::IntRect(spriteSheetRectPosition.x, spriteSheetRectPosition.y, ENEMY_WIDTH, ENEMY_HEIGHT),
                 spriteSheetRectPosition, STARTING_ENEMY_HEALTH),
      player(player),
      levelDimensions(levelDimensions),
      enemySpritePositionOffsetX((std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT) * 0.5f)),
      enemySpritePositionOffsetY((std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT)))
{
    sf::Texture& texture = *(textureManager->getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY));
    entitySprite = sf::Sprite(texture, entitySpriteSheetDimRect);
    entitySprite.scale(ENEMY_SCALE_FACTOR , ENEMY_SCALE_FACTOR);
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
    uint32_t tileUnderEnemyX = floor((position.x + enemySpritePositionOffsetX) / TILE_SIZE);
    uint32_t tileUnderEnemyY = floor((position.y + enemySpritePositionOffsetY) / TILE_SIZE);

    tilePosition = sf::Vector2u(tileUnderEnemyX, tileUnderEnemyY);
    sf::Time deltaTime = gameState.getClock().getDeltaTime();

    // TODO REFACTOR BELOW
    int milliseconds = gameState.getClock().getWorldTimeMs();
    if (milliseconds > entityWaitTimeBeforeMovement)
    {
        if (isEnemyInProximityOfTarget(position.x, position.y, player->getPosition().x, player->getPosition().y,
                                       WANDER_DISTANCE))
        {
            moveToDestination(deltaTime.asSeconds(), player->getPosition().x, player->getPosition().y);
            if (isEnemyInProximityOfTarget(position.x, position.y, player->getPosition().x, player->getPosition().y, 24)) {
                damagePlayer(gameState.getClock().getWorldTimeSeconds());
            }
        }
        else
        {
            if (isEnemyInProximityOfTarget(position.x, position.y, spawnPosition.x, spawnPosition.y, WANDER_DISTANCE))
            {
                // Move randomly
                updatePosition(deltaTime, levelDimensions.x, levelDimensions.y);
                if (milliseconds > (entityWaitTimeBeforeMovement + 250))
                {
                    entityWaitTimeBeforeMovement = std::experimental::randint(milliseconds + MIN_ENEMY_MOVE_RATE, milliseconds + MAX_ENEMY_MOVE_RATE);
                    updateEntityToRandomDirection();
                }
            }
            else
            {
                // Return to spawn area if too far away.
                moveToDestination(deltaTime.asSeconds(), spawnPosition.x, spawnPosition.y);
            }
        }

        uint32_t spriteSheetTop = startingAnimationPosition.y + (ENEMY_HEIGHT * getSpriteSheetAnimationOffset(direction));
        uint32_t spriteSheetLeft = entitySpriteSheetDimRect.left == (startingAnimationPosition.x + (ENEMY_WIDTH * 2))
                ? startingAnimationPosition.x
                : entitySpriteSheetDimRect.left + ENEMY_WIDTH;
        updateAnimation(deltaTime, spriteSheetTop, spriteSheetLeft);
    }

    entitySprite.setPosition(position);
    entitySprite.setTextureRect(entitySpriteSheetDimRect);
}

void Enemy::damagePlayer(const float worldTimeSeconds)
{
    uint16_t playerHealth = player->getHealth();
    int timeNowSeconds = static_cast<int>(worldTimeSeconds);
    bool hasEnemyAttackedAfterTimeWindow = (timeNowSeconds < 3 || timeNowSeconds - lastTimeEnemyAttacked >= 3);

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

void Enemy::moveToDestination(const float deltaTimeSeconds, float destinationX, float destinationY) {
    velocity += sf::Vector2f(speed, speed);
    sf::Vector2f newPosition = getPosition();

    bool isEnemyToLeftOfDestination = newPosition.x < destinationX;
    float positionDeltaX = isEnemyToLeftOfDestination
            ? (newPosition.x + (velocity.x * deltaTimeSeconds))
            : (newPosition.x - (velocity.x * deltaTimeSeconds));

    bool isEnemyAboveDestination = newPosition.y < destinationY;
    float positionDeltaY = isEnemyAboveDestination
            ? (newPosition.y + (velocity.y * deltaTimeSeconds))
            : (newPosition.y - (velocity.y * deltaTimeSeconds));

    bool isEnemyWithinRangeOfPlayerOnYAxis =
            abs(newPosition.y - player->getPosition().y) < HORIZONTAL_DIRECTION_WINDOW_SIZE_FOR_ENEMY_ANIMATION;

    if (isEnemyWithinRangeOfPlayerOnYAxis)
    {
        direction = isEnemyToLeftOfDestination ? EntityDirection::RIGHT : EntityDirection::LEFT;
    }
    else
    {
        direction = isEnemyAboveDestination ? EntityDirection::DOWN : EntityDirection::UP;
    }

    newPosition.x = positionDeltaX;
    newPosition.y = positionDeltaY;
    setPosition(newPosition);

    velocity.x *= 0.15;
    velocity.y *= 0.15;
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
