#pragma once

#ifndef VANQUISH_ENEMY_HEALTH_BAR_H
#define VANQUISH_ENEMY_HEALTH_BAR_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "health_bar.h"
#include "game_entity.h"

class EnemyHealthBar : public HealthBar
{
    public:
        EnemyHealthBar(const std::shared_ptr<GameEntity>& entity, sf::Vector2f fillDimensions,
                sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
                float outlineThickness);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(GameState& gameState) override;

        std::shared_ptr<GameEntity> getEntity() const;

};

#endif //VANQUISH_ENEMY_HEALTH_BAR_H
