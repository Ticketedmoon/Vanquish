#pragma once

#include <filesystem>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <experimental/random>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "game_component.h"

enum class EntityDirection 
{
    DOWN = 0,
    UP = 1,
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
        GameEntity(uint8_t width, uint8_t height, float speed, sf::Vector2f position, sf::IntRect entitySpriteSheetDimRect,
                   sf::Vector2f startingAnimationPosition, uint16_t health);
        ~GameEntity() override = default;

        // Not necessary to add virtual here to maintain pure-virtual function as parent method is pure-virtual.
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override = 0;
        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override = 0;

        virtual void updateAnimation(sf::Time& deltaTime, uint32_t spriteSheetTop, uint32_t spriteSheetLeft);
        virtual sf::Time getAnimationFrameDuration() = 0;

        virtual EntityType getType() = 0;

        void updateEntityToRandomDirection();
        void updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight);
        void setDirection(EntityDirection dir);

        static uint8_t getSpriteSheetAnimationOffset(EntityDirection dir);
        EntityDirection getDirection() const;
        uint8_t getWidth() const;
        uint8_t getHeight() const;

        bool isDead() const;
        uint16_t getHealth() const;
        void setHealth(uint16_t newHealth);

        sf::Vector2u tilePosition;
        sf::Vector2f velocity;
        sf::IntRect entitySpriteSheetDimRect;

    protected:
        float speed;

        sf::Vector2f spawnPosition;

        EntityDirection direction = EntityDirection::DOWN;

        sf::Sprite entitySprite;

        sf::Vector2f startingAnimationPosition;
        sf::Time animationFrameStartTime{ sf::Time::Zero };

    private:
        uint16_t health;
        uint8_t width;
        uint8_t height;

        struct NextCoordinateVelocityPair
        {
            float coordinatePosition;
            float velocity;
        };

        static NextCoordinateVelocityPair getNextCoordinatePositionWithNextVelocity(const sf::Time &deltaTime,
                                                                                    uint32_t tileMapDimensionValue,
                                                                                    float positionForCoordinate,
                                                                                    float velocityForCoordinate);
};
