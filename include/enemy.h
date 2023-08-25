#pragma once

#include <experimental/random>

#include "game_entity.h"
#include "texture_manager.h"
#include "player.h"
#include "common_constants.h"

class Enemy : public GameEntity
{
    public:
        explicit Enemy(TextureManager& textureManager, std::shared_ptr<Player>& player,
                       uint32_t posX, uint32_t posY, int rectLeft, int rectTop);

        Enemy(Enemy& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Enemy` GameEntity object: " << &enemy << std::endl;
        }
        Enemy(Enemy&& enemy)  noexcept
        {
            std::cout << "DEBUG: copy constructor called for `Enemy` GameEntity object: " << &enemy << std::endl;
        }

        // TODO ADD DELTA TIME TO CONSTRUCTOR RATHER THAN NEEDING TO PASS IT IN EACH METHOD
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTopOffset) override;
        void reset() override;

        void updateEntityToRandomDirection();

        static uint32_t constexpr ENEMY_WIDTH = 32;
        static uint32_t constexpr ENEMY_HEIGHT = 32;

        // TODO REFACTOR
        int entityWaitTimeBeforeMovement = std::experimental::randint(5000, 10000);
        int directionIndex = std::experimental::randint(0, 3);

    private:
        static bool isEnemyInProximityOfTarget(float sourceLocationX, float sourceLocationY, float targetLocationX, float targetLocationY,
                                        uint32_t distance);
        void moveToDestination(const sf::Time &deltaTime, float destinationX, float destinationY);

        std::shared_ptr<Player> player;

        static constexpr uint32_t WANDER_DISTANCE = 256;
        static float constexpr ENEMY_SCALE_FACTOR = 0.75;
        static float constexpr ENEMY_SPEED = 45.0f;

        // TODO Move me to parent class?
        uint32_t enemySpritePositionOffsetX{};
        uint32_t enemySpritePositionOffsetY{};
};
