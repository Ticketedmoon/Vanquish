#pragma once

#include "game_entity.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Player : public GameEntity
{
    public:
        Player();

        void update() override;
        void updatePosition(uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Clock& clock, uint32_t spriteSheetTopOffset, EntityDirection newDirection) override;

        std::pair<uint32_t, uint32_t> findNextTileFromPlayerDirection(EntityDirection playerDir);

        sf::Vector2f velocity;
        sf::Vector2u tilePosition;

        static uint32_t constexpr PLAYER_WIDTH = 32;
        static uint32_t constexpr PLAYER_HEIGHT = 32;

    private:
        static float constexpr PLAYER_SCALE_X = 0.75;
        static float constexpr PLAYER_SCALE_Y = 0.75;
        static float constexpr PLAYER_SPEED = 1.5f;

        uint32_t playerSpritePositionOffsetX;
        uint32_t playerSpritePositionOffsetY;
};
