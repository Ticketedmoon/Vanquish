#include "../include/player.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

Player::Player()
{
    if (!texture.loadFromFile("resources/assets/character_sprite_sheet_v2.png"))
    {
        std::cout << "Failed to load character sprite sheet" << std::endl;
    }

    texture.setSmooth(true);
    playerPos = sf::Vector2f(0, 0);
    rectSourceSprite = sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    playerSprite = sf::Sprite(texture, rectSourceSprite);
    playerSprite.scale(PLAYER_SCALE_X, PLAYER_SCALE_X);

    playerSpritePositionOffsetY = floor(PLAYER_SCALE_Y * PLAYER_HEIGHT);
    playerSpritePositionOffsetX = playerSpritePositionOffsetY * 0.5f;
}

void Player::update()
{
    uint32_t tileUnderPlayerX = floor((playerPos.x+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t tileUnderPlayerY = floor((playerPos.y+playerSpritePositionOffsetY)/PLAYER_HEIGHT);

    tilePosition = sf::Vector2u(tileUnderPlayerX, tileUnderPlayerY);
    playerSprite.setPosition(playerPos);
    playerSprite.setTextureRect(rectSourceSprite);
}

std::pair<uint32_t, uint32_t> Player::findNextTileFromPlayerDirection(PlayerDirection playerDir)
{
    float nextPlayerX = playerPos.x;
    float nextPlayerY = playerPos.y;

    if (playerDir == PlayerDirection::UP)
    {
        nextPlayerY += (velocity.y - PLAYER_SPEED);
    }
    if (playerDir == PlayerDirection::DOWN)
    {
        nextPlayerY += (velocity.y + PLAYER_SPEED);
    }
    if (playerDir == PlayerDirection::LEFT)
    {
        nextPlayerX += (velocity.x - PLAYER_SPEED);
    }
    if (playerDir == PlayerDirection::RIGHT) 
    {
        nextPlayerX += (velocity.x + PLAYER_SPEED);
    }
    
    uint32_t nextTileX = floor((nextPlayerX+playerSpritePositionOffsetX)/PLAYER_WIDTH);
    uint32_t nextTileY = floor((nextPlayerY+playerSpritePositionOffsetY)/PLAYER_HEIGHT);
    return std::pair<uint32_t, uint32_t>(nextTileX, nextTileY);
}

// TODO REFACTOR ME
void Player::updatePlayerPosition(uint32_t levelWidth, uint32_t levelHeight)
{
    const int sign = playerDir == PlayerDirection::UP || playerDir == PlayerDirection::LEFT ? -1 : 1;

    int xAccel = playerDir == PlayerDirection::LEFT || playerDir == PlayerDirection::RIGHT ? (PLAYER_SPEED * sign) : 0;
    int yAccel = playerDir == PlayerDirection::UP || playerDir == PlayerDirection::DOWN ? (PLAYER_SPEED * sign) : 0;
    sf::Vector2f acceleration = sf::Vector2f(xAccel, yAccel);

    // update velocity through acceleration
    velocity += acceleration;

    // TODO REFACTOR ME CAN WE USE tilePosition?
    uint32_t tileMapWidth = ((levelWidth-1) * PLAYER_WIDTH);
    uint32_t tileMapHeight = ((levelHeight-1) * PLAYER_HEIGHT);

    if (playerDir == PlayerDirection::LEFT || playerDir == PlayerDirection::RIGHT)
    {
        if ((playerPos.x+velocity.x >= 0) && (playerPos.x+velocity.x < tileMapWidth))
        {
            playerPos.x += velocity.x;
            velocity.x = 0.5f * velocity.x;
        }
        else 
        {
            velocity.x = 0;
        }
    }

    if (playerDir == PlayerDirection::UP || playerDir == PlayerDirection::DOWN)
    {
        if ((playerPos.y+velocity.y >= 0) && (playerPos.y+velocity.y <= tileMapHeight))
        {
            playerPos.y += velocity.y;
            velocity.y = 0.5f * velocity.y;
        }
        else
        {
            velocity.y = 0;
        }
    }
}

void Player::updateAnimation(sf::Clock& clock, uint32_t spriteSheetTopOffset, PlayerDirection newDirection)
{
    sf::Time currentTime = clock.getElapsedTime();
    if (currentTime - animationFrameStartTime >= animationFrameDuration)
    {
        rectSourceSprite.top = PLAYER_HEIGHT * spriteSheetTopOffset;
        if (rectSourceSprite.left == (PLAYER_WIDTH*2))
        {
            rectSourceSprite.left = 0;
        }
        else
        {
            rectSourceSprite.left += PLAYER_WIDTH;
        }

        animationFrameStartTime = currentTime;
    }
    playerDir = newDirection;
}

sf::Vector2f Player::getPlayerPos()
{
    return playerPos;
}

PlayerDirection Player::getPlayerDir()
{
    return playerDir;
}

sf::Sprite Player::getSprite()
{
    return playerSprite;
}

