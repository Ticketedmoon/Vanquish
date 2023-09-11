#include "../include/view_manager.h"

ViewManager::ViewManager(sf::RenderWindow& window, Level& level, std::shared_ptr<TextManager>& textManager)
        : m_window(window), m_level(level), m_textManager(textManager)
{
}

void ViewManager::centerViewOnEntity(const std::shared_ptr<GameEntity>& entity)
{
    // keep view centred/centered on entity
    sf::Vector2f entityPos = entity->getPosition();
    float centreX = getViewCentreForCoordinate(entityPos.x, m_level.getLevelWidth(), WINDOW_WIDTH, entity->getWidth());
    float centreY = getViewCentreForCoordinate(entityPos.y, m_level.getLevelHeight(), WINDOW_HEIGHT, entity->getHeight());

    sf::View newView = m_window.getView();
    newView.zoom(VIEW_ZOOM_FACTOR);
    newView.setCenter(centreX, centreY);

    m_window.setView(newView);
}

float ViewManager::getViewCentreForCoordinate(const float playerCoordinatePosition, const float levelDimension,
                                              const float windowDimensionValue, const float playerDimensionValue) {
    if (playerCoordinatePosition <= windowDimensionValue / 4)
    {
        return windowDimensionValue / 4;
    }

    return playerCoordinatePosition < (playerDimensionValue * levelDimension)
           ? playerCoordinatePosition
           : windowDimensionValue - (windowDimensionValue / 3);
}
