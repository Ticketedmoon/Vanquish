#pragma once

#ifndef VANQUISH_PLAYER_H
#define VANQUISH_PLAYER_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include "game_entity.h"
#include "texture_manager.h"
#include "common_constants.h"

inline static constexpr uint8_t PLAYER_WIDTH = 48;
inline static constexpr uint8_t PLAYER_HEIGHT = 50;
inline static constexpr float PLAYER_SCALE_FACTOR = 0.85;
inline static constexpr float PLAYER_SPEED = 60.0f;
inline static constexpr uint16_t STARTING_PLAYER_HEALTH = 200;

class Player : public GameEntity
{
    public:
        explicit Player(std::shared_ptr<TextureManager>& textureManager);
        Player(Player& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Player` GameEntity object: " << &enemy << std::endl;
        }

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(GameClock& gameClock) override;
        void updateAnimation(sf::Time deltaTime, uint32_t spriteSheetTop, uint32_t spriteSheetLeft) override;
        sf::Time getAnimationFrameDuration() override;

        EntityType getType() override;

        sf::Vector2<uint32_t> findNextTileFromPlayerDirection(sf::Time deltaTime, EntityDirection playerDir);

    private:
        uint32_t playerSpritePositionOffsetX;
        uint32_t playerSpritePositionOffsetY;

        sf::Time animationFrameDuration{ sf::seconds(1.f / 16.f) };
};

#endif //VANQUISH_PLAYER_H