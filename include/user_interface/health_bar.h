#pragma once

#ifndef VANQUISH_HEALTH_BAR_H
#define VANQUISH_HEALTH_BAR_H

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
        void update(GameState& gameState) override;

    private:
        std::shared_ptr<GameEntity> entity;
        sf::RectangleShape healthBar;
        sf::RectangleShape healthBarBackground;

};

#endif //VANQUISH_HEALTH_BAR_H
