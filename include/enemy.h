#pragma once

#include "game_entity.h"

#include <experimental/random>

class Enemy : public GameEntity
{
    public:
        explicit Enemy(uint32_t posX, uint32_t posY, int rectLeft, int rectTop);

        Enemy(Enemy& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Enemy` GameEntity object: " << &enemy << std::endl;
        }
        Enemy(Enemy&& enemy)  noexcept
        {
            std::cout << "DEBUG: copy constructor called for `Enemy` GameEntity object: " << &enemy << std::endl;
        }

        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTopOffset) override;
        void updateEntityToRandomDirection();

        static uint32_t constexpr ENEMY_WIDTH = 32;
        static uint32_t constexpr ENEMY_HEIGHT = 32;

        // TODO REFACTOR
        int entityWaitTimeBeforeMovement = std::experimental::randint(5000, 10000);
        int directionIndex = std::experimental::randint(0, 3);

    private:
        static float constexpr ENEMY_SCALE_X = 0.75;
        static float constexpr ENEMY_SCALE_Y = 0.75;
        static float constexpr ENEMY_SPEED = 45.0f;

        // TODO Move me to parent class?
        uint32_t enemySpritePositionOffsetX{};
        uint32_t enemySpritePositionOffsetY{};
};
