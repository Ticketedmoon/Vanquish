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
#include <utility>

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
        void updatePlayerPosition(uint32_t levelWidth, uint32_t levelHeight);
        void updateAnimation(sf::Clock& clock, uint32_t spriteSheetTopOffset, PlayerDirection newDirection);
        std::pair<uint32_t, uint32_t> findNextTileFromPlayerDirection(PlayerDirection playerDir);

        // TODO IS THIS STILL NEEDED?
        sf::Vector2f getPlayerPos();
        sf::Sprite getSprite();
        PlayerDirection getPlayerDir();

        sf::Vector2f velocity;
        sf::Vector2u tilePosition;

    private:
        static float constexpr playerScaleX = 0.75;
        static float constexpr playerScaleY = 0.75;

        static uint32_t constexpr playerWidth = 32;
        static uint32_t constexpr playerHeight = 32;

        uint32_t playerSpritePositionOffsetX;
        uint32_t playerSpritePositionOffsetY;

        sf::Vector2f playerPos;
        PlayerDirection playerDir = PlayerDirection::DOWN;

        sf::IntRect rectSourceSprite;
        sf::Texture texture;
        sf::Sprite playerSprite;

        sf::Time animationFrameStartTime{ sf::Time::Zero };
        sf::Time animationFrameDuration{ sf::seconds(1.f / 6.f) }; // 3 frames per second
};
