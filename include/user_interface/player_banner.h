#pragma once

#include <memory>
#include "game_component.h"
#include "game_entity.h"
#include "text_manager.h"

#ifndef VANQUISH_PLAYER_BANNER_H
#define VANQUISH_PLAYER_BANNER_H

class PlayerBanner : public GameComponent
{
    public:
        PlayerBanner(std::shared_ptr<GameEntity> gameEntity, sf::Vector2f fillDimensions,
                sf::Vector2f backgroundDimensions, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColour,
                sf::Color backgroundFillColour, float outlineThickness, std::shared_ptr<TextManager> textManager);
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
        void update(GameState& gameState) override;

    private:
        std::shared_ptr<GameEntity> entity;
        sf::RectangleShape banner;
        sf::RectangleShape bannerBackground;
        std::shared_ptr<TextManager> textManager;
};

#endif //VANQUISH_PLAYER_BANNER_H
