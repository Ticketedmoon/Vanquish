#pragma once

#include <math.h>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <string.h>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>

enum class EntityDirection 
{
    DOWN = 0,
    UP = 1,
    LEFT = 2,
    RIGHT = 3
};

class GameEntity
{
    public:
        virtual void update() = 0;
        virtual void updatePosition(uint32_t levelWidth, uint32_t levelHeight) = 0;
        // TODO Should this be virtual?
        virtual void updateAnimation(sf::Clock& clock, uint32_t spriteSheetTopOffset, EntityDirection newDirection) = 0;

        sf::Vector2f getPosition();
        sf::Sprite getSprite();
        EntityDirection getDirection();

    protected:
        sf::Vector2f position;
        EntityDirection direction = EntityDirection::DOWN;
        sf::Time animationFrameStartTime{ sf::Time::Zero };
        sf::Time animationFrameDuration{ sf::seconds(1.f / 6.f) }; // 3 frames per second
        sf::IntRect rectSourceEntity;
        sf::Texture texture;
        sf::Sprite entitySprite;
};
