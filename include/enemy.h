#pragma once

#ifndef VANQUISH_ENEMY_H
#define VANQUISH_ENEMY_H

#include "game_entity.h"
#include "player.h"
#include "texture_manager.h"
#include "common_constants.h"

static constexpr uint8_t ENEMY_WIDTH = 32;
static constexpr uint8_t ENEMY_HEIGHT = 32;
static constexpr uint32_t WANDER_DISTANCE = 128;
static constexpr float ENEMY_SCALE_FACTOR = 0.75;
static constexpr float ENEMY_SPEED = 45.0f;

static constexpr int HORIZONTAL_DIRECTION_WINDOW_SIZE_FOR_ENEMY_ANIMATION = 20;

static inline constexpr uint16_t STARTING_ENEMY_HEALTH = 50;

class Enemy : public GameEntity
{
    public:
        explicit Enemy(std::shared_ptr<TextureManager>& textureManager, std::shared_ptr<Player>& player,
                       sf::Vector2f position, sf::Vector2u spriteSheetRectPosition);

        Enemy(Enemy& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Enemy` GameEntity object: " << &enemy << std::endl;
        }

        // TODO ADD DELTA TIME TO CONSTRUCTOR RATHER THAN NEEDING TO PASS IT IN EACH METHOD
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(GameState& gameState) override;

        EntityType getType() override;

    private:
        sf::Time getAnimationFrameDuration() override;
        static bool isEnemyInProximityOfTarget(sf::Vector2f sourceLocation, sf::Vector2f targetLocation, uint32_t distance);
        void moveToDestination(GameClock& gameClock, sf::Vector2f destinationPoint);
        void damagePlayer(GameClock& gameClock);

    private:
        static constexpr uint8_t MAX_SPRITE_SHEET_FRAMES = 3;

        int lastTimeEnemyAttacked = 0;

        // TODO ENEMY SHOULDN't HAVE REF TO PLAYER
        std::shared_ptr<Player> m_player;

        // TODO IMPROVE THIS, MAKE MORE DYNAMIC
        uint16_t damage = std::experimental::randint(5, 20);

        sf::Time animationFrameDuration{ sf::seconds(1.f / 6.f) }; // 3 frames per second
};

#endif //VANQUISH_ENEMY_H