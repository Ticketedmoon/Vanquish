#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include "game_entity.h"
#include "texture_manager.h"

class Player : public GameEntity
{
    public:
        explicit Player(TextureManager& textureManager);
        Player(Player& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Player` GameEntity object: " << &enemy << std::endl;
        }
        Player(Player&& enemy)  noexcept
        {
            std::cout << "DEBUG: copy constructor called for `Player` GameEntity object: " << &enemy << std::endl;
        }

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTopOffset) override;

        std::pair<uint32_t, uint32_t> findNextTileFromPlayerDirection(sf::Time& deltaTime, EntityDirection playerDir);

        // TODO MAKE THIS PRIVATE WITH GETTERS
        static constexpr uint32_t PLAYER_WIDTH = 32;
        static constexpr uint32_t PLAYER_HEIGHT = 32;
        static constexpr uint16_t HEALTH = 100;

    private:

        static constexpr float PLAYER_SCALE_X = 0.75;
        static constexpr float PLAYER_SCALE_Y = 0.75;
        static constexpr float PLAYER_SPEED = 50.0f;

        uint32_t playerSpritePositionOffsetX;
        uint32_t playerSpritePositionOffsetY;
};
