#pragma once

#ifndef VANQUISH_HEALTH_BAR_H
#define VANQUISH_HEALTH_BAR_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "game_entity.h"

class HealthBar : public GameComponent
{
    public:
        HealthBar(const std::shared_ptr<GameEntity>& entity, sf::Vector2f fillDimensions,
                sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
                float outlineThickness);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override = 0;
        void update(GameState& gameState) override = 0;

    protected:
        std::shared_ptr<GameEntity> entity;
        sf::RectangleShape healthBar;
        sf::RectangleShape healthBarBackground;

};

#endif //VANQUISH_HEALTH_BAR_H
