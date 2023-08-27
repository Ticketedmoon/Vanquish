#include "../include/enemy.h"

Enemy::Enemy(TextureManager& textureManager, std::shared_ptr<Player>& player, uint32_t posX, uint32_t posY, int rectLeft, int rectTop)
    : GameEntity(ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_SPEED, sf::Vector2f(posX, posY),
                 sf::IntRect(rectLeft, rectTop, ENEMY_WIDTH, ENEMY_HEIGHT),
                 sf::Vector2f(rectLeft, rectTop)),
      player(player),
      enemySpritePositionOffsetX((std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT) * 0.5f)),
      enemySpritePositionOffsetY((std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT)))
{
    sf::Texture& texture = *(textureManager.getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY));
    entitySprite = sf::Sprite(texture, entityDimRect);
    entitySprite.scale(ENEMY_SCALE_FACTOR , ENEMY_SCALE_FACTOR);
}

void Enemy::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    uint32_t tileUnderEnemyX = floor((position.x + enemySpritePositionOffsetX) / ENEMY_WIDTH);
    uint32_t tileUnderEnemyY = floor((position.y + enemySpritePositionOffsetY) / ENEMY_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderEnemyX, tileUnderEnemyY);
    entitySprite.setPosition(position);
    entitySprite.setTextureRect(entityDimRect);

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
            if (isEnemyInProximityOfTarget(position.x, position.y, spawnPosition.x,
                                           spawnPosition.y, WANDER_DISTANCE))
            {
                // Move randomly
                updateEntityToRandomDirection();
                updatePosition(deltaTime, levelWidth, levelHeight);
                if (milliseconds > (entityWaitTimeBeforeMovement + 250))
                {
                    entityWaitTimeBeforeMovement = std::experimental::randint(milliseconds + 5000,milliseconds + 10000);
                    directionIndex = std::experimental::randint(0, 3);
                }
            }
            else
            {
                // Return to spawn area if too far away.
                moveToDestination(deltaTime, spawnPosition.x, spawnPosition.y);
            }
        }

        uint32_t spriteSheetTop = startingAnimationPosition.y + (ENEMY_HEIGHT * getSpriteSheetAnimationOffset(direction));
        uint32_t spriteSheetLeft = entityDimRect.left == (startingAnimationPosition.x + (ENEMY_WIDTH * 2))
                ? startingAnimationPosition.x
                : entityDimRect.left + ENEMY_WIDTH;
        updateAnimation(worldClock, spriteSheetTop, spriteSheetLeft);
    }
}

void Enemy::damagePlayer(const sf::Clock& worldClock)
{
    uint16_t playerHealth = player->getHealth();
    int timeNowSeconds = static_cast<int>(worldClock.getElapsedTime().asSeconds());
    bool hasPlayerBeenAttackedWithinValidTimeWindow = (timeNowSeconds < 3
            || timeNowSeconds - lastTimePlayerWasHitSeconds >= 3);

    if (playerHealth > 0 && hasPlayerBeenAttackedWithinValidTimeWindow) {
        // TODO RESEARCH IF WE NEED DELTA TIME SINCE WE ARE ALREADY USING WORLD CLOCK
        uint16_t newHealth = playerHealth - damage > 0 ? playerHealth - damage : 0;
        player->setHealth(newHealth);
        lastTimePlayerWasHitSeconds = timeNowSeconds;
    }
}

void Enemy::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(entitySprite);
}

void Enemy::moveToDestination(const sf::Time &deltaTime, float destinationX, float destinationY) {
    velocity += sf::Vector2f(ENEMY_SPEED, ENEMY_SPEED);

    bool isEnemyToLeftOfDestination = position.x < destinationX;
    float positionDeltaX = isEnemyToLeftOfDestination
            ? (position.x + (velocity.x * deltaTime.asSeconds()))
            : (position.x - (velocity.x * deltaTime.asSeconds()));

    bool isEnemyAboveDestination = position.y < destinationY;
    float positionDeltaY = isEnemyAboveDestination
            ? (position.y + (velocity.y * deltaTime.asSeconds()))
            : (position.y - (velocity.y * deltaTime.asSeconds()));

    bool isEnemyWithinRangeOfPlayerOnYAxis =
            abs(position.y - player->getPosition().y) < HORIZONTAL_DIRECTION_WINDOW_SIZE_FOR_ENEMY_ANIMATION;

    if (isEnemyWithinRangeOfPlayerOnYAxis)
    {
        direction = isEnemyToLeftOfDestination ? EntityDirection::RIGHT : EntityDirection::LEFT;
    }
    else
    {
        direction = isEnemyAboveDestination ? EntityDirection::DOWN : EntityDirection::UP;
    }

    position.x = positionDeltaX;
    position.y = positionDeltaY;

    velocity.x *= 0.15;
    velocity.y *= 0.15;
}

bool Enemy::isEnemyInProximityOfTarget(float sourceLocationX, float sourceLocationY, float targetLocationX,
                                       float targetLocationY, uint32_t distance)
{
    return sqrt(pow(sourceLocationX - targetLocationX, 2) + pow(sourceLocationY - targetLocationY, 2)) < distance;
}

void Enemy::updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTopOffset, uint32_t spriteSheetLeftOffset)
{
    sf::Time currentTime = worldClock.getElapsedTime();
    if (currentTime - animationFrameStartTime >= animationFrameDuration)
    {
        entityDimRect.top = spriteSheetTopOffset;
        entityDimRect.left = spriteSheetLeftOffset;
        animationFrameStartTime = currentTime;
    }
}

void Enemy::updateEntityToRandomDirection()
{
    EntityDirection newDir = directionIndex == 0
            ? EntityDirection::UP : directionIndex == 1
            ? EntityDirection::RIGHT : directionIndex == 2
            ? EntityDirection::DOWN : EntityDirection::LEFT;

    this->setDirection(newDir);
}

void Enemy::reset() {
    GameEntity::reset();
    entityDimRect = sf::IntRect(startingAnimationPosition.x, startingAnimationPosition.y, ENEMY_WIDTH, ENEMY_HEIGHT);
    enemySpritePositionOffsetY = std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT);
    enemySpritePositionOffsetX = enemySpritePositionOffsetY * 0.5f;
}
