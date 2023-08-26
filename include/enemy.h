#pragma once

#include <experimental/random>

#include "game_entity.h"
#include "texture_manager.h"
#include "player.h"
#include "common_constants.h"

static constexpr uint8_t ENEMY_WIDTH = 32;
static constexpr uint8_t ENEMY_HEIGHT = 32;
static constexpr uint32_t WANDER_DISTANCE = 256;
static constexpr float ENEMY_SCALE_FACTOR = 0.75;
static constexpr float ENEMY_SPEED = 45.0f;

class Enemy : public GameEntity
{
    public:
        explicit Enemy(TextureManager& textureManager, std::shared_ptr<Player>& player,
                       uint32_t posX, uint32_t posY, int rectLeft, int rectTop);

        Enemy(Enemy& enemy) : GameEntity(enemy)
        {
            std::cout << "DEBUG: copy constructor called for `Enemy` GameEntity object: " << &enemy << std::endl;
        }

        // TODO ADD DELTA TIME TO CONSTRUCTOR RATHER THAN NEEDING TO PASS IT IN EACH METHOD
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;
        void updateAnimation(sf::Clock& worldClock, uint32_t spriteSheetTopOffset, uint32_t spriteSheetLeftOffset) override;
        void reset() override;

        void updateEntityToRandomDirection();

        // TODO REFACTOR
        int entityWaitTimeBeforeMovement = std::experimental::randint(5000, 10000);
        int directionIndex = std::experimental::randint(0, 3);
        int lastTimePlayerWasHitSeconds = 0;

    private:
        static bool isEnemyInProximityOfTarget(float sourceLocationX, float sourceLocationY, float targetLocationX, float targetLocationY,
                                        uint32_t distance);
        void moveToDestination(const sf::Time &deltaTime, float destinationX, float destinationY);

        std::shared_ptr<Player> player;

        // TODO IMPROVE THIS, MAKE MORE DYNAMIC
        int damage = std::experimental::randint(5, 20);

        // TODO Move me to parent class?
        uint32_t enemySpritePositionOffsetX{};
        uint32_t enemySpritePositionOffsetY{};
};
