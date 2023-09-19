#pragma once

#ifndef VANQUISH_EXPERIENCE_POINTS_BAR_H
#define VANQUISH_EXPERIENCE_POINTS_BAR_H


#include <memory>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "game_entity.h"
#include "player.h"

class ExperiencePointsBar : public GameComponent
{
    public:
        ExperiencePointsBar(const std::shared_ptr<Player>& player, sf::Vector2f fillDimensions,
                sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
                float outlineThickness);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(GameState& gameState) override;

    private:
        std::shared_ptr<GameEntity> m_player;
        sf::RectangleShape experienceBar;
        sf::RectangleShape experienceBarBackground;
};


#endif //VANQUISH_EXPERIENCE_POINTS_BAR_H
