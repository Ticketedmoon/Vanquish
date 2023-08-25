#include "../include/enemy.h"

Enemy::Enemy(TextureManager& textureManager, std::shared_ptr<Player>& player, uint32_t posX, uint32_t posY, int rectLeft, int rectTop)
    : player(player),
      enemySpritePositionOffsetX((std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT) * 0.5f)),
      enemySpritePositionOffsetY((std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT)))
{
    spawnPosition = sf::Vector2f(posX, posY);
    position = spawnPosition;
    startingAnimationPosition = sf::Vector2f(rectLeft, rectTop);

    rectSourceEntity = sf::IntRect(startingAnimationPosition.x, startingAnimationPosition.y, ENEMY_WIDTH, ENEMY_HEIGHT);

    sf::Texture& texture = *(textureManager.getTexture(HUMAN_CHARACTER_SPRITE_SHEET_A_KEY));
    entitySprite = sf::Sprite(texture, rectSourceEntity);
    entitySprite.scale(ENEMY_SCALE_FACTOR , ENEMY_SCALE_FACTOR);
}

void Enemy::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    uint32_t tileUnderEnemyX = floor((position.x+enemySpritePositionOffsetX)/ENEMY_WIDTH);
    uint32_t tileUnderEnemyY = floor((position.y+enemySpritePositionOffsetY)/ENEMY_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderEnemyX, tileUnderEnemyY);
    entitySprite.setPosition(position);
    entitySprite.setTextureRect(rectSourceEntity);

    // TODO REFACTOR BELOW
    int milliseconds = worldClock.getElapsedTime().asMilliseconds();
    if (milliseconds > entityWaitTimeBeforeMovement)
    {
        if (isEnemyInProximityOfTarget(position.x, position.y,player->getPosition().x, player->getPosition().y,
                                       WANDER_DISTANCE))
        {
            moveToDestination(deltaTime, player->getPosition().x, player->getPosition().y);
            if (isEnemyInProximityOfTarget(position.x, position.y, player->getPosition().x, player->getPosition().y, 24)) {
                uint16_t playerHealth = player->getHealth();
                if (playerHealth > 0) {
                    // TODO IMPROVE CALCULATION + DELTA TIME
                    uint16_t newHealth = playerHealth - 1;
                    player->setHealth(newHealth);
                }
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

    position.x = positionDeltaX;
    position.y = positionDeltaY;

    velocity.x *= 0.15;
    velocity.y *= 0.15;
}

bool Enemy::isEnemyInProximityOfTarget(float sourceLocationX, float sourceLocationY, float targetLocationX, float targetLocationY,
                                       uint32_t distance)
{
    return sqrt(pow(sourceLocationX - targetLocationX, 2) + pow(sourceLocationY - targetLocationY, 2) * 1.0) < distance;
}

// TODO REFACTOR ME
// TODO SAME AS method in player.h, can be centralised.
void Enemy::updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    const float sign = direction == EntityDirection::UP || direction == EntityDirection::LEFT ? -1.0f : 1.0f;
    float xAccel = direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT ? (ENEMY_SPEED * sign) : 0;
    float yAccel = direction == EntityDirection::UP || direction == EntityDirection::DOWN ? (ENEMY_SPEED * sign) : 0;
    sf::Vector2f acceleration = sf::Vector2f(xAccel, yAccel);

    // update velocity through acceleration
    velocity += acceleration;

    // TODO REFACTOR ME CAN WE USE tilePosition?
    uint32_t tileMapWidth = ((levelWidth-1) * ENEMY_WIDTH);
    uint32_t tileMapHeight = ((levelHeight-1) * ENEMY_HEIGHT);

    if (direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT)
    {
        float positionDeltaX = position.x + (velocity.x * deltaTime.asSeconds());
        if (positionDeltaX >= 0 && positionDeltaX <static_cast<float>(tileMapWidth))
        {
            position.x = positionDeltaX;
            velocity.x *= 0.5f;
        }
        else
        {
            velocity.x = 0;
        }
    }

    if (direction == EntityDirection::UP || direction == EntityDirection::DOWN)
    {
        float positionDeltaY = position.y + (velocity.y * deltaTime.asSeconds());
        if (positionDeltaY >= 0 && positionDeltaY <static_cast<float>(tileMapHeight))
        {
            position.y = positionDeltaY;
            velocity.y *= 0.5f;
        }
        else
        {
            velocity.y = 0;
        }
    }
}

void Enemy::updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTopOffset)
{
    sf::Time currentTime = worldClock.getElapsedTime();
    if (currentTime - animationFrameStartTime >= animationFrameDuration)
    {
        rectSourceEntity.top = ENEMY_HEIGHT * spriteSheetTopOffset;
        rectSourceEntity.left = (rectSourceEntity.left == (ENEMY_WIDTH*2)) ? 0 : (rectSourceEntity.left + ENEMY_WIDTH);
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
    rectSourceEntity = sf::IntRect(startingAnimationPosition.x, startingAnimationPosition.y, ENEMY_WIDTH, ENEMY_HEIGHT);
    enemySpritePositionOffsetY = std::floor(ENEMY_SCALE_FACTOR * ENEMY_HEIGHT);
    enemySpritePositionOffsetX = enemySpritePositionOffsetY * 0.5f;
}
