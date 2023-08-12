#pragma once

#include "game_entity.h"
#include <cstdint>

class Enemy : public GameEntity
{
    public:
        Enemy(uint32_t x, uint32_t y);

        void update() override;
        void updatePosition(uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Clock& clock, uint32_t spriteSheetTopOffset, EntityDirection newDirection) override;

        sf::Vector2f velocity;
        sf::Vector2u tilePosition;

        static uint32_t constexpr ENEMY_WIDTH = 32;
        static uint32_t constexpr ENEMY_HEIGHT = 32;

    private:
        static float constexpr ENEMY_SCALE_X = 0.75;
        static float constexpr ENEMY_SCALE_Y = 0.75;
        static float constexpr ENEMY_SPEED = 1.5f;

        // TODO Move me to parent class?
        uint32_t enemySpritePositionOffsetX;
        uint32_t enemySpritePositionOffsetY;
};
