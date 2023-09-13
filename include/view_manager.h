#pragma once

#ifndef VANQUISH_VIEW_MANAGER_H
#define VANQUISH_VIEW_MANAGER_H

#include <SFML/Graphics/Text.hpp>

#include "../include/game_entity.h"
#include "../include/common_constants.h"
#include "../include/level.h"
#include "../include/player.h"
#include "../include/text_manager.h"
#include "user_interface_manager.h"

static const float VIEW_ZOOM_FACTOR = 0.65;

class ViewManager
{
    public:
        ViewManager(sf::RenderWindow& window, Level& level, std::shared_ptr<TextManager>& textManager);
        void centerViewOnEntity(const std::shared_ptr<GameEntity>& entity);
        void showGameOverView();

    private:
        static float getViewCentreForCoordinate(float playerCoordinatePosition, float levelDimension,
                                                float windowDimensionValue, float playerDimensionValue);

    private:
        sf::RenderWindow& m_window;
        Level& m_level;
        std::shared_ptr<TextManager> m_textManager;
};

#endif //VANQUISH_VIEW_MANAGER_H