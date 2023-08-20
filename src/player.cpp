#include "../include/player.h"

Player::Player()
{
    if (!texture.loadFromFile("resources/assets/character_sprite_sheet_v2.png"))
    {
        std::cout << "Failed to load character sprite sheet, current path: "  << std::filesystem::current_path() << std::endl;
    }

    texture.setSmooth(true);
    position = sf::Vector2f(0, 0);
    rectSourceEntity = sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    entitySprite = sf::Sprite(texture, rectSourceEntity);
    entitySprite.scale(PLAYER_SCALE_X, PLAYER_SCALE_X);

    playerSpritePositionOffsetY = std::floor(PLAYER_SCALE_Y * PLAYER_HEIGHT);
    playerSpritePositionOffsetX = playerSpritePositionOffsetY * 0.5f;
}

void Player::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    uint32_t tileUnderPlayerX = floor((position.x+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t tileUnderPlayerY = floor((position.y+playerSpritePositionOffsetY)/PLAYER_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderPlayerX, tileUnderPlayerY);

    entitySprite.setPosition(position);
    entitySprite.setTextureRect(rectSourceEntity);
}

std::pair<uint32_t, uint32_t> Player::findNextTileFromPlayerDirection(sf::Time& deltaTime, EntityDirection direction)
{
    float nextPlayerX = position.x;
    float nextPlayerY = position.y;

    if (direction == EntityDirection::UP)
    {
        nextPlayerY += (velocity.y - PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::DOWN)
    {
        nextPlayerY += (velocity.y + PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::LEFT)
    {
        nextPlayerX += (velocity.x - PLAYER_SPEED) * deltaTime.asSeconds();
    }
    if (direction == EntityDirection::RIGHT) 
    {
        nextPlayerX += (velocity.x + PLAYER_SPEED) * deltaTime.asSeconds();
    }
    
    uint32_t nextTileX = std::floor((nextPlayerX+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t nextTileY = std::floor((nextPlayerY+playerSpritePositionOffsetY)/PLAYER_HEIGHT);
    return std::pair{nextTileX, nextTileY};
}

// TODO REFACTOR ME
void Player::updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
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

void Player::updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTopOffset)
{
    sf::Time currentWorldTime = worldClock.getElapsedTime();
    if (currentWorldTime - animationFrameStartTime >= animationFrameDuration)
    {
        rectSourceEntity.top = PLAYER_HEIGHT * spriteSheetTopOffset;
        rectSourceEntity.left = (rectSourceEntity.left == (PLAYER_WIDTH*2)) ? 0 : (rectSourceEntity.left + PLAYER_WIDTH);
        animationFrameStartTime = currentWorldTime;
    }
}