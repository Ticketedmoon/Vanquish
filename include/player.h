#pragma once

#include "game_entity.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

class Player : public GameEntity
{
    public:
        Player();
        Player(Player& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Player` GameEntity object: " << &enemy << std::endl;
        }
        Player(Player&& enemy)  noexcept
        {
            std::cout << "DEBUG: copy constructor called for `Player` GameEntity object: " << &enemy << std::endl;
        }

        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTopOffset) override;

        std::pair<uint32_t, uint32_t> findNextTileFromPlayerDirection(sf::Time& deltaTime, EntityDirection playerDir);

        static uint32_t constexpr PLAYER_WIDTH = 32;
        static uint32_t constexpr PLAYER_HEIGHT = 32;

    private:
        static float constexpr PLAYER_SCALE_X = 0.75;
        static float constexpr PLAYER_SCALE_Y = 0.75;
        static float constexpr PLAYER_SPEED = 50.0f;

        uint32_t playerSpritePositionOffsetX;
        uint32_t playerSpritePositionOffsetY;
};
