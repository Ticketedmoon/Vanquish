#include "../include/enemy.h"

Enemy::Enemy(TextureManager& textureManager, std::shared_ptr<Player>& player, uint32_t posX, uint32_t posY, uint32_t rectLeft, uint32_t rectTop)
    : GameEntity(ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_SPEED, sf::Vector2f(posX, posY),
                 sf::IntRect(rectLeft, rectTop, ENEMY_WIDTH, ENEMY_HEIGHT),
                 sf::Vector2f(rectLeft, rectTop)),
      player(player),
      enemySpritePositionOffsetX((std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT) * 0.5f)),
      enemySpritePositionOffsetY((std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT)))
{
    sf::Texture& texture = *(textureManager.getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY));
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

void Enemy::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    const sf::Vector2f& position = getPosition();
    uint32_t tileUnderEnemyX = floor((position.x + enemySpritePositionOffsetX) / ENEMY_WIDTH);
    uint32_t tileUnderEnemyY = floor((position.y + enemySpritePositionOffsetY) / ENEMY_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderEnemyX, tileUnderEnemyY);

    // TODO REFACTOR BELOW
    int milliseconds = worldClock.getElapsedTime().asMilliseconds();
    if (milliseconds > entityWaitTimeBeforeMovement)
    {
        if (isEnemyInProximityOfTarget(position.x, position.y, player->getPosition().x, player->getPosition().y,
                                       WANDER_DISTANCE))
        {
            moveToDestination(deltaTime, player->getPosition().x, player->getPosition().y);
            if (isEnemyInProximityOfTarget(position.x, position.y, player->getPosition().x, player->getPosition().y, 24)) {
                damagePlayer(worldClock);
            }
        }
        else
        {
            if (isEnemyInProximityOfTarget(position.x, position.y, spawnPosition.x, spawnPosition.y, WANDER_DISTANCE))
            {
                // Move randomly
                updatePosition(deltaTime, levelWidth, levelHeight);
                if (milliseconds > (entityWaitTimeBeforeMovement + 250))
                {
                    entityWaitTimeBeforeMovement = std::experimental::randint(milliseconds + 5000, milliseconds + 10000);
                    updateEntityToRandomDirection();
                }
            }
            else
            {
                // Return to spawn area if too far away.
                moveToDestination(deltaTime, spawnPosition.x, spawnPosition.y);
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

void Enemy::damagePlayer(const sf::Clock& worldClock)
{
    uint16_t playerHealth = player->getHealth();
    int timeNowSeconds = static_cast<int>(worldClock.getElapsedTime().asSeconds());
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

void Enemy::moveToDestination(const sf::Time &deltaTime, float destinationX, float destinationY) {
    velocity += sf::Vector2f(speed, speed);
    sf::Vector2f newPosition = getPosition();

    bool isEnemyToLeftOfDestination = newPosition.x < destinationX;
    float positionDeltaX = isEnemyToLeftOfDestination
            ? (newPosition.x + (velocity.x * deltaTime.asSeconds()))
            : (newPosition.x - (velocity.x * deltaTime.asSeconds()));

    bool isEnemyAboveDestination = newPosition.y < destinationY;
    float positionDeltaY = isEnemyAboveDestination
            ? (newPosition.y + (velocity.y * deltaTime.asSeconds()))
            : (newPosition.y - (velocity.y * deltaTime.asSeconds()));

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

void Enemy::reset() {
    GameEntity::reset();
    entitySpriteSheetDimRect = sf::IntRect(startingAnimationPosition.x, startingAnimationPosition.y, ENEMY_WIDTH, ENEMY_HEIGHT);
    enemySpritePositionOffsetY = std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT);
    enemySpritePositionOffsetX = enemySpritePositionOffsetY * 0.5f;
}

EntityType Enemy::getType()
{
    return EntityType::ENEMY;
}