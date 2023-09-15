#pragma once

#ifndef VANQUISH_GAME_ENTITY_H
#define VANQUISH_GAME_ENTITY_H

#include <filesystem>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <experimental/random>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "game_component.h"

static constexpr uint16_t MIN_ENTITY_MOVE_RATE_MS = 500;
static constexpr uint16_t MAX_ENTITY_MOVE_RATE_MS = 3000;

enum class EntityDirection
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

enum class EntityType
{
    PLAYER,
    ENEMY
};

class GameEntity : public GameComponent
{
    public:
        GameEntity(uint8_t width, uint8_t height, float speed, sf::Vector2f position,
                sf::IntRect entitySpriteSheetDimRect, sf::Vector2u startingAnimationPosition, uint16_t health,
                sf::Vector2u spritePositionOffset);
        ~GameEntity() override = default;

        // Not necessary to add virtual here to maintain pure-virtual function as parent method is pure-virtual.
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override = 0;
        void update(GameState& gameState) override = 0;

        virtual EntityType getType() = 0;

        sf::Vector2<uint32_t> findNextTileDirection(sf::Time deltaTime);

        uint8_t getWidth() const;
        uint8_t getHeight() const;

        uint16_t getHealth() const;
        void setHealth(uint16_t newHealth);

    protected:
        virtual sf::Time getAnimationFrameDuration() = 0;
        virtual void updateAnimation(sf::Time deltaTime, uint32_t spriteSheetTop, uint32_t spriteSheetLeft);
        static uint8_t getSpriteSheetAnimationOffset(EntityDirection dir);
        void performAnimation(GameClock& gameClock, uint8_t maxSpriteSheetFrames);

        void updatePosition(GameClock& gameClock);
        void setDirection(EntityDirection dir);
        void updateEntityToRandomDirection(GameClock& gameClock, uint8_t maxSpriteSheetFrames);

        bool isDead() const;

    private:
        struct NextCoordinateVelocityPair
        {
            float coordinatePosition;
            float velocity;
        };

        static NextCoordinateVelocityPair getNextCoordinatePositionWithNextVelocity(sf::Time deltaTime,
                uint32_t tileMapDimensionValue,
                float positionForCoordinate,
                float velocityForCoordinate);

        bool isNextTileCollidable(GameClock& gameClock);

    public:
        sf::Vector2u tilePosition;

    protected:
        uint8_t width;
        uint8_t height;
        float speed;
        sf::Vector2f velocity;
        uint16_t health;
        EntityDirection direction = EntityDirection::DOWN;

        sf::Sprite entitySprite;
        sf::Vector2f spawnPosition;
        uint16_t entityWaitTimeBeforeMovementMs = std::experimental::randint(MIN_ENTITY_MOVE_RATE_MS,
                MAX_ENTITY_MOVE_RATE_MS);

        sf::IntRect entitySpriteSheetDimRect;
        sf::Vector2u spritePositionOffset;
        sf::Vector2i startingAnimationPosition;
        sf::Time animationFrameStartTime{sf::Time::Zero};
};

#endif //VANQUISH_GAME_ENTITY_H