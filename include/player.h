#pragma once

#ifndef VANQUISH_PLAYER_H
#define VANQUISH_PLAYER_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include "game_entity.h"
#include "game_state.h"
#include "common_constants.h"
#include "texture_manager.h"

static inline constexpr uint8_t PLAYER_WIDTH = 48;
static inline constexpr uint8_t PLAYER_HEIGHT = 50;
static inline constexpr float PLAYER_SCALE_FACTOR = 0.85;
static inline constexpr float PLAYER_SPEED = 60.0f;
static inline constexpr uint16_t STARTING_PLAYER_HEALTH = 200;

class Player : public GameEntity
{
    public:
        explicit Player(std::shared_ptr<TextureManager>& textureManager);
        Player(Player& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Player` GameEntity object: " << &enemy << std::endl;
        }

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(GameState& gameState) override;

        EntityType getType() override;

    private:
        sf::Time getAnimationFrameDuration() override;
        void startMovement(GameClock& gameClock, EntityDirection direction, uint8_t spriteSheetTopOffset);

    private:
        static constexpr uint8_t MAX_SPRITE_SHEET_FRAMES = 7;
        sf::Time animationFrameDuration{sf::seconds(1.f / 16.f)};

};

#endif //VANQUISH_PLAYER_H