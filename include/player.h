#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include "game_entity.h"
#include "texture_manager.h"
#include "common_constants.h"

inline static constexpr uint8_t PLAYER_WIDTH = 32;
inline static constexpr uint8_t PLAYER_HEIGHT = 32;
inline static constexpr float PLAYER_SCALE_FACTOR = 0.75;
inline static constexpr float PLAYER_SPEED = 50.0f;
inline static constexpr uint16_t STARTING_PLAYER_HEALTH = 200;

class Player : public GameEntity
{
    public:
        explicit Player(TextureManager& textureManager);
        Player(Player& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Player` GameEntity object: " << &enemy << std::endl;
        }

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Time& deltaTime, uint32_t spriteSheetTop, uint32_t spriteSheetLeft) override;
        void reset() override;

        EntityType getType() override;

        bool isDead() const;
        uint16_t getHealth() const;
        void setHealth(uint16_t newHealth);

        sf::Vector2<uint32_t> findNextTileFromPlayerDirection(sf::Time& deltaTime, EntityDirection playerDir);

    private:
        uint16_t health = STARTING_PLAYER_HEALTH;

        uint32_t playerSpritePositionOffsetX;
        uint32_t playerSpritePositionOffsetY;
};
