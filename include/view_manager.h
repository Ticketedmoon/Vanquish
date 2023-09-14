#pragma once

#ifndef VANQUISH_VIEW_MANAGER_H
#define VANQUISH_VIEW_MANAGER_H

#include <SFML/Graphics/Text.hpp>

#include "game_entity.h"
#include "common_constants.h"
#include "level.h"
#include "player.h"
#include "text_manager.h"
#include "user_interface_manager.h"

static const float VIEW_ZOOM_FACTOR = 0.5;

class ViewManager
{
    public:
        ViewManager(sf::RenderTarget& renderTarget, Level& level, std::shared_ptr<TextManager>& textManager);
        void centerViewOnEntity(const std::shared_ptr<GameEntity>& entity);
        void showGameOverView();

    private:
        static float getViewCentreForCoordinate(float playerCoordinatePosition, float levelDimension,
                                                float windowDimensionValue, float playerDimensionValue);

    private:
        sf::RenderTarget& m_renderTarget;
        Level& m_level;
        std::shared_ptr<TextManager> m_textManager;
};

#endif //VANQUISH_VIEW_MANAGER_H