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
        Player(Player& player) : GameEntity(player)
        {
            std::cout << "DEBUG: copy constructor called for `Player` GameEntity object: " << &player << '\n';
        }

        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(GameState& gameState) override;

        EntityType getType() override;
        void damageTarget(GameClock& gameClock, const std::shared_ptr<GameEntity>& target) override;
        void addExperiencePoints(std::shared_ptr<GameEntity>& entityKilled);

    private:
        void startMovement(GameState& gameState);
        bool tryMoveDirection(GameClock& gameClock, std::pair<sf::Keyboard::Key, sf::Keyboard::Key> keyboardInputGroup,
                EntityDirection direction);
        void startAnimation(GameClock& gameClock, const std::string& animationKeyA, const std::string& animationKeyB);
        void startAnimationFromAnimationGroup(GameClock& gameClock, const std::string& animationKey);
        void applyDamage(GameClock& gameClock, uint16_t damage) override;

    private:
        std::shared_ptr<TextureManager> m_textureManager;

        static constexpr float HURT_ANIMATION_TIME_OFFSET = 0.25;

        // TODO refactor these, centralise in some way and perhaps try use the same timer for each action.
        float deathTimeSeconds = 0.0f;
        float painTimeSeconds = 0.0f;
        float swingTimeSeconds = 0.0f;
};

#endif //VANQUISH_PLAYER_H