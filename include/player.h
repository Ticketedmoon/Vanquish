#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <cmath>

static uint32_t SPRITE_SHEET_X = 32;
static uint32_t SPRITE_SHEET_Y = 32;
static uint32_t LAST_SPRITE_LEFT_POS = 64;

// TODO STILL NEEDED?
static uint32_t PLAYER_SPEED = 32;

enum class PlayerDirection { 
    DOWN = 0,
    UP = 1,
    LEFT = 2,
    RIGHT = 3
};

class Player
{
    public:
        Player();
        void update();

        sf::Vector2f getPlayerPos();
        sf::Sprite getSprite();
        PlayerDirection getPlayerDir();

        void updatePlayerPosition();
        void updateAnimation(sf::Clock& clock, uint32_t spriteSheetTopOffset, PlayerDirection newDirection);

        sf::Vector2f velocity;

    private:
        sf::IntRect rectSourceSprite;
        sf::Vector2f playerPos;
        sf::Texture texture;
        sf::Sprite playerSprite;

        PlayerDirection playerDir;

        sf::Time animationFrameStartTime{ sf::Time::Zero };
        sf::Time animationFrameDuration{ sf::seconds(1.f / 6.f) }; // 3 frames per second
};
