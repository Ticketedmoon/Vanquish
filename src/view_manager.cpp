#include "../include/view_manager.h"

ViewManager::ViewManager(sf::RenderTarget& renderTarget, Level& level, std::shared_ptr<TextManager>& textManager)
        : m_renderTarget(renderTarget), m_level(level), m_textManager(textManager)
{
}

void ViewManager::centerViewOnEntity(const std::shared_ptr<GameEntity>& entity)
{
    // keep view centred/centered on entity
    sf::Vector2f entityPos = entity->getPosition();
    float centreX = getViewCentreForCoordinate(entityPos.x, m_level.getWorldWidth(), WINDOW_WIDTH, entity->getWidth());
    float centreY = getViewCentreForCoordinate(entityPos.y, m_level.getWorldHeight(), WINDOW_HEIGHT, entity->getHeight());

    sf::View newView = m_renderTarget.getView();
    newView.zoom(VIEW_ZOOM_FACTOR);
    newView.setCenter(centreX, centreY);

    m_renderTarget.setView(newView);
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

void ViewManager::showGameOverView() {
    m_renderTarget.clear(sf::Color::Red);
    const std::string text = "You have died!\nPress [SPACE] to restart";

    // TODO - Make this calculation using text.getLocalBounds() or View config
    float textPositionX = WINDOW_WIDTH / 2 - (text.size() * 0.5 * 20);
    m_textManager->drawText(text, sf::Color::White, 64, sf::Vector2f(textPositionX, 255));
}
