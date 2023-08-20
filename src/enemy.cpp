#include "../include/enemy.h"

Enemy::Enemy(uint32_t posX, uint32_t posY, int rectLeft, int rectTop)
{
    if (!texture.loadFromFile("resources/assets/character_sprite_sheet_v2.png"))
    {
        std::cout << "Failed to load character sprite sheet, current path: "  << std::filesystem::current_path() << std::endl;
    }

    texture.setSmooth(true);
    position = sf::Vector2f(posX, posY);

    rectSourceEntity = sf::IntRect(rectLeft, rectTop, ENEMY_WIDTH, ENEMY_HEIGHT);
    entitySprite = sf::Sprite(texture, rectSourceEntity);
    entitySprite.scale(ENEMY_SCALE_X , ENEMY_SCALE_X);

    enemySpritePositionOffsetY = std::floor(ENEMY_SCALE_Y * ENEMY_HEIGHT);
    enemySpritePositionOffsetX = enemySpritePositionOffsetY * 0.5f;
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
        updateEntityToRandomDirection();
        updatePosition(deltaTime, levelWidth, levelHeight);
        if (milliseconds > (entityWaitTimeBeforeMovement + 250))
        {
            entityWaitTimeBeforeMovement = std::experimental::randint(milliseconds + 5000,milliseconds + 10000);
            directionIndex = std::experimental::randint(0, 3);
        }
    }

}

// TODO REFACTOR ME
// TODO SAME AS method in player.h, can be centralised.
void Enemy::updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    const int sign = direction == EntityDirection::UP || direction == EntityDirection::LEFT ? -1 : 1;
    int xAccel = direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT ? (ENEMY_SPEED * sign) : 0;
    int yAccel = direction == EntityDirection::UP || direction == EntityDirection::DOWN ? (ENEMY_SPEED * sign) : 0;
    sf::Vector2f acceleration = sf::Vector2f(xAccel, yAccel);

    // update velocity through acceleration
    velocity += acceleration;

    // TODO REFACTOR ME CAN WE USE tilePosition?
    uint32_t tileMapWidth = ((levelWidth-1) * ENEMY_WIDTH);
    uint32_t tileMapHeight = ((levelHeight-1) * ENEMY_HEIGHT);

    if (direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT)
    {
        float positionDeltaX = position.x + (velocity.x * deltaTime.asSeconds());
        if (positionDeltaX >= 0 && positionDeltaX < static_cast<float>(tileMapWidth))
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
        if (positionDeltaY >= 0 && positionDeltaY < static_cast<float>(tileMapHeight))
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

