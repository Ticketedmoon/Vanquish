#include "../include/player.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

Player::Player(uint8_t playerWidth, uint8_t playerHeight)
{
    this->playerWidth = playerWidth;
    this->playerHeight = playerHeight;
    playerDir = PlayerDirection::DOWN;

    if (!texture.loadFromFile("resources/assets/character_sprite_sheet_v2.png"))
    {
        std::cout << "Failed to load character sprite sheet" << std::endl;
    }

    texture.setSmooth(true);
    playerPos = sf::Vector2f(0, 0);
    rectSourceSprite = sf::IntRect(0, 0, playerWidth, playerHeight);
    playerSprite = sf::Sprite(texture, rectSourceSprite);
}

void Player::update()
{
    uint32_t tileUnderPlayerX = round(playerPos.x/playerWidth);
    uint32_t tileUnderPlayerY = round(playerPos.y/playerHeight);

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
        nextPlayerY += (velocity.y - 1.5f);
    }
    if (playerDir == PlayerDirection::DOWN)
    {
        nextPlayerY += (velocity.y + 1.5f);
    }
    if (playerDir == PlayerDirection::LEFT)
    {
        nextPlayerX += (velocity.x - 1.5f);
    }
    if (playerDir == PlayerDirection::RIGHT) 
    {
        nextPlayerX += (velocity.x + 1.5f);
    }

    // TODO ROUND FUNC IS A PROBLEM HERE, MAYBE USE WITHOUT AGG FUNC? 
    // CASE: 0 - 16 first tilePosition
    // CASE: 16 - 32 second tilePosition
    // CASE: 32 - 48 third tilePosition
    //
    // Currently it deems 0-16 tile 1, and 16-48 tile 2, due to rounding behaviour.
    // We want to divide by 16 and floor maybe? but X breaks
    
    uint32_t nextTileX = round(nextPlayerX/playerWidth);
    uint32_t nextTileY = round(nextPlayerY/playerHeight);
    return std::pair<uint32_t, uint32_t>(nextTileX, nextTileY);
}

// TODO REFACTOR ME
void Player::updatePlayerPosition(uint32_t levelWidth, uint32_t levelHeight)
{
    const float dAcc = 1.5f;
    const int sign = playerDir == PlayerDirection::UP || playerDir == PlayerDirection::LEFT ? -1 : 1;

    int xAccel = playerDir == PlayerDirection::LEFT || playerDir == PlayerDirection::RIGHT ? (dAcc * sign) : 0;
    int yAccel = playerDir == PlayerDirection::UP || playerDir == PlayerDirection::DOWN ? (dAcc * sign) : 0;
    sf::Vector2f acceleration = sf::Vector2f(xAccel, yAccel);

    // update velocity through acceleration
    velocity += acceleration;

    // TODO REFACTOR ME CAN WE USE tilePosition?
    uint32_t tileMapWidth = ((levelWidth-1) * playerWidth);
    uint32_t tileMapHeight = ((levelHeight-1) * playerHeight);

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
        rectSourceSprite.top = playerHeight * spriteSheetTopOffset;
        if (rectSourceSprite.left == (playerWidth*2))
        {
            rectSourceSprite.left = 0;
        }
        else
        {
            rectSourceSprite.left += playerWidth;
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

