#include "../include/player.h"
#include <SFML/System/Vector2.hpp>

Player::Player()
{
    if (!texture.loadFromFile("resources/assets/character_sprite_sheet_v2.png"))
    {
        std::cout << "Failed to load character sprite sheet" << std::endl;
    }
    texture.setSmooth(true);

    playerPos = sf::Vector2f(0.0f, 0.0f);
    rectSourceSprite = sf::IntRect(0, 0, SPRITE_SHEET_X, SPRITE_SHEET_Y);
    playerDir = PlayerDirection::DOWN;
    playerSprite = sf::Sprite(texture, rectSourceSprite);
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

void Player::updatePlayerPosition()
{
    if (playerDir == PlayerDirection::UP)
    {
        playerPos.y -= PLAYER_SPEED;
    }
    if (playerDir == PlayerDirection::DOWN)
    {
        playerPos.y += PLAYER_SPEED;
    }
    if (playerDir == PlayerDirection::LEFT)
    {
        playerPos.x -= PLAYER_SPEED;
    }
    if (playerDir == PlayerDirection::RIGHT) 
    {
        playerPos.x += PLAYER_SPEED;
    }
}

void Player::updateAnimation(uint32_t spriteSheetTopOffset, PlayerDirection newDirection)
{
    rectSourceSprite.top = SPRITE_SHEET_X * spriteSheetTopOffset;
    if (rectSourceSprite.left == LAST_SPRITE_LEFT_POS)
    {
        rectSourceSprite.left = 0;
    }
    else
    {
        rectSourceSprite.left += SPRITE_SHEET_X;
    }
    playerDir = newDirection;
}

// TODO A better solution might be to temporarily update player position in tmp vars.
//      If validation passes, commit update player pos.
//      Otherwise, reject and don't move player.
void Player::update()
{
    playerSprite.setPosition(playerPos);

    playerSprite.setTextureRect(rectSourceSprite);
}
