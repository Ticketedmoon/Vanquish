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
#include "tile.h"
#include "texture_manager.h"
#include "animation_manager.h"
#include "animation_group.h"

static constexpr uint16_t MIN_ENTITY_MOVE_RATE_MS = 500;
static constexpr uint16_t MAX_ENTITY_MOVE_RATE_MS = 3000;

enum class EntityDirection
{
    DOWN = 0,
    LEFT = 1,
    RIGHT = 2,
    UP = 3,
};

enum class EntityType
{
    PLAYER,
    ENEMY
};

class GameEntity : public GameComponent
{
    public:
        GameEntity(uint8_t width, uint8_t height, float speed, sf::Vector2f position, uint16_t health,
                sf::Vector2u spritePositionOffset, std::unordered_map<std::string, std::shared_ptr<AnimationGroup>> animationGroup);
        ~GameEntity() override = default;

        // Not necessary to add virtual here to maintain pure-virtual function as parent method is pure-virtual.
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override = 0;
        void update(GameState& gameState) override = 0;

        virtual EntityType getType() = 0;
        virtual void applyDamage(GameClock& gameClock, uint16_t damage);

        sf::Vector2u findNextTileFromDirection(sf::Time deltaTime) const;
        void updateSpriteColour(sf::Color spriteColour);

        uint8_t getWidth() const;
        uint8_t getHeight() const;
        EntityDirection getDirection() const;
        uint16_t getHealth() const;
        bool isDead() const;

    protected:
        void performAnimationByKey(GameClock& gameClock, const std::string& animationKey);

        void updatePosition(GameClock& gameClock);
        void setDirection(EntityDirection dir);
        void updateEntityToRandomDirection(GameClock& gameClock, std::string animationKey);

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

        bool isNextTileCollidable(GameClock& gameClock) const;

    public:
        sf::Vector2u tilePosition;

        // FIXME temporary
        float lastTimeTakenDamageSeconds = 0.0f;

    protected:
        uint8_t width;
        uint8_t height;

        float speed;
        sf::Vector2f velocity;

        EntityDirection direction = EntityDirection::DOWN;
        std::unordered_map<std::string, std::shared_ptr<AnimationGroup>> animationGroupMap;

        uint16_t health;

        sf::Sprite entitySprite;
        sf::Vector2u spritePositionOffset;
        sf::Vector2f spawnPosition;

        // TODO Move to enemy class?
        uint16_t waitTimeBeforeMovementMs = std::experimental::randint(MIN_ENTITY_MOVE_RATE_MS, MAX_ENTITY_MOVE_RATE_MS);
};

#endif //VANQUISH_GAME_ENTITY_H