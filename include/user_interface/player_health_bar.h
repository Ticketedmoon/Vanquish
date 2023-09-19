#pragma once

#ifndef VANQUISH_PLAYER_HEALTH_BAR_H
#define VANQUISH_PLAYER_HEALTH_BAR_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "game_entity.h"
#include "health_bar.h"

class PlayerHealthBar : public HealthBar
{
    public:
        PlayerHealthBar(const std::shared_ptr<GameEntity>& entity, sf::Vector2f fillDimensions,
                sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
                float outlineThickness);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(GameState& gameState) override;

};

#endif //VANQUISH_PLAYER_HEALTH_BAR_H
