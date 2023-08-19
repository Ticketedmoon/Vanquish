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

void Enemy::update()
{
    uint32_t tileUnderEnemyX = floor((position.x+enemySpritePositionOffsetX)/ENEMY_WIDTH);
    uint32_t tileUnderEnemyY = floor((position.y+enemySpritePositionOffsetY)/ENEMY_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderEnemyX, tileUnderEnemyY);
    entitySprite.setPosition(position);
    entitySprite.setTextureRect(rectSourceEntity);
}

// TODO REFACTOR ME
void Enemy::updatePosition(uint32_t levelWidth, uint32_t levelHeight)
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
        if ((position.x+velocity.x >= 0) && (position.x+velocity.x < tileMapWidth))
        {
            position.x += velocity.x;
            velocity.x = 0.5f * velocity.x;
        }
        else 
        {
            velocity.x = 0;
        }
    }

    if (direction == EntityDirection::UP || direction == EntityDirection::DOWN)
    {
        if ((position.y+velocity.y >= 0) && (position.y+velocity.y <= tileMapHeight))
        {
            position.y += velocity.y;
            velocity.y = 0.5f * velocity.y;
        }
        else
        {
            velocity.y = 0;
        }
    }
}

void Enemy::updateAnimation(sf::Clock& clock, uint32_t spriteSheetTopOffset)
{
    sf::Time currentTime = clock.getElapsedTime();
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

