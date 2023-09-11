#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "game_entity.h"

class HealthBar : public GameComponent
{
    public:
        HealthBar(std::shared_ptr<GameEntity>& gameEntity, sf::Vector2f dimensions, sf::Vector2f position,
                  sf::Color fillColor, sf::Color outlineColour,
                  float outlineThickness);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight) override;

    private:
        std::shared_ptr<GameEntity> entity;
        sf::RectangleShape healthBar;
        sf::RectangleShape healthBarBackground;

};