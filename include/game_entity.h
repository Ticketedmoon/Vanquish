#pragma once

#include <filesystem>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstring>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

enum class EntityDirection 
{
    DOWN = 0,
    UP = 1,
    LEFT = 2,
    RIGHT = 3
};

class GameEntity : public sf::Drawable
{
    public:
        ~GameEntity() override = default;

        // Not necessary to add virtual here ti maintain pure-virtual function as parent method is pure-virtual.
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override = 0;
        virtual void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) = 0;
        virtual void updatePosition(sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) = 0;
        virtual void updateAnimation(sf::Clock &worldClock, uint32_t spriteSheetTopOffset) = 0;

        void setDirection(EntityDirection dir);

        sf::Vector2f getPosition();
        sf::Sprite& getSprite();
        EntityDirection getDirection();

        sf::Vector2f velocity;
        sf::Vector2u tilePosition;

    protected:
        sf::Vector2f position;
        EntityDirection direction = EntityDirection::DOWN;

        sf::IntRect rectSourceEntity;
        sf::Sprite entitySprite;

        sf::Time animationFrameStartTime{ sf::Time::Zero };
        sf::Time animationFrameDuration{ sf::seconds(1.f / 6.f) }; // 3 frames per second
};
