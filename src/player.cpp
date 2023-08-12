#include "../include/player.h"

Player::Player()
{
    if (!texture.loadFromFile("resources/assets/character_sprite_sheet_v2.png"))
    {
        std::cout << "Failed to load character sprite sheet" << std::endl;
    }

    texture.setSmooth(true);
    position = sf::Vector2f(0, 0);
    rectSourceEntity = sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    entitySprite = sf::Sprite(texture, rectSourceEntity);
    entitySprite.scale(PLAYER_SCALE_X, PLAYER_SCALE_X);

    playerSpritePositionOffsetY = floor(PLAYER_SCALE_Y * PLAYER_HEIGHT);
    playerSpritePositionOffsetX = playerSpritePositionOffsetY * 0.5f;
}

void Player::update()
{
    uint32_t tileUnderPlayerX = floor((position.x+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t tileUnderPlayerY = floor((position.y+playerSpritePositionOffsetY)/PLAYER_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderPlayerX, tileUnderPlayerY);
    entitySprite.setPosition(position);
    entitySprite.setTextureRect(rectSourceEntity);
}

std::pair<uint32_t, uint32_t> Player::findNextTileFromPlayerDirection(EntityDirection direction)
{
    float nextPlayerX = position.x;
    float nextPlayerY = position.y;

    if (direction == EntityDirection::UP)
    {
        nextPlayerY += (velocity.y - PLAYER_SPEED);
    }
    if (direction == EntityDirection::DOWN)
    {
        nextPlayerY += (velocity.y + PLAYER_SPEED);
    }
    if (direction == EntityDirection::LEFT)
    {
        nextPlayerX += (velocity.x - PLAYER_SPEED);
    }
    if (direction == EntityDirection::RIGHT) 
    {
        nextPlayerX += (velocity.x + PLAYER_SPEED);
    }
    
    uint32_t nextTileX = floor((nextPlayerX+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t nextTileY = floor((nextPlayerY+playerSpritePositionOffsetY)/PLAYER_HEIGHT);
    return std::pair<uint32_t, uint32_t>(nextTileX, nextTileY);
}

// TODO REFACTOR ME
void Player::updatePosition(uint32_t levelWidth, uint32_t levelHeight)
{
    const int sign = direction == EntityDirection::UP || direction == EntityDirection::LEFT ? -1 : 1;

    int xAccel = direction == EntityDirection::LEFT || direction == EntityDirection::RIGHT ? (PLAYER_SPEED * sign) : 0;
    int yAccel = direction == EntityDirection::UP || direction == EntityDirection::DOWN ? (PLAYER_SPEED * sign) : 0;
    sf::Vector2f acceleration = sf::Vector2f(xAccel, yAccel);

    // update velocity through acceleration
    velocity += acceleration;

    // TODO REFACTOR ME CAN WE USE tilePosition?
    uint32_t tileMapWidth = ((levelWidth-1) * PLAYER_WIDTH);
    uint32_t tileMapHeight = ((levelHeight-1) * PLAYER_HEIGHT);

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

void Player::updateAnimation(sf::Clock& clock, uint32_t spriteSheetTopOffset, EntityDirection newDirection)
{
    sf::Time currentTime = clock.getElapsedTime();
    if (currentTime - animationFrameStartTime >= animationFrameDuration)
    {
        rectSourceEntity.top = PLAYER_HEIGHT * spriteSheetTopOffset;
        if (rectSourceEntity.left == (PLAYER_WIDTH*2))
        {
            rectSourceEntity.left = 0;
        }
        else
        {
            rectSourceEntity.left += PLAYER_WIDTH;
        }

        animationFrameStartTime = currentTime;
    }
    direction = newDirection;
}
