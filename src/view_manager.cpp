#include "../include/view_manager.h"

ViewManager::ViewManager(sf::RenderWindow& window, Level& level, std::shared_ptr<TextManager>& textManager)
    : m_window(window), m_level(level), m_textManager(textManager)
{
}

void ViewManager::centerViewOnEntity(const std::shared_ptr<GameEntity>& entity)
{
    // kee view centred/centered on player
    sf::Vector2f playerPos = entity->getPosition();
    float centreX = getViewCentreForCoordinate(playerPos.x, m_level.getLevelWidth(), WINDOW_WIDTH, entity->getWidth());
    float centreY = getViewCentreForCoordinate(playerPos.y, m_level.getLevelHeight(), WINDOW_HEIGHT, entity->getHeight());

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

/*
 * TODO ADD IMPROVEMENT TO ENABLE DEBUG CODE ONLY IF CODE IS RUN IN DEBUG CONFIGURATION.
 *      WE DO NOT WANT TO HAVE DEBUG CODE IN DISTRIBUTION BUILDS.
 * TODO This has nothing to do with an sf::view, so maybe move to a DebugManager or in RenderingManager
 */
void ViewManager::startDebugView(sf::Clock& debugClock, std::shared_ptr<Player>& player)
{
    std::string fps = std::to_string(1.0f / debugClock.restart().asSeconds());
    const std::string& playerPosX = std::to_string(player->getPosition().x);
    const std::string& playerPosY = std::to_string(player->getPosition().y);
    const std::string& playerTileX = std::to_string(player->tilePosition.x);
    const std::string& playerTileY = std::to_string(player->tilePosition.y);

    std::string text =
            "fps: " + fps + "\n" +
            "window position (x, y): " + "(" + playerPosX + ", " + playerPosY + ")" + "\n" +
            "tile position (x, y): " + "(" + playerTileX + ", " + playerTileY + ")" + "\n";

    m_textManager->drawText(text, sf::Color::Green, 12, sf::Vector2f(25, 50));

    m_level.enableEntityTileHighlightsForDebug({
        {EntityType::PLAYER, sf::Color{0, 196, 128, 255}},
        {EntityType::ENEMY, sf::Color{255, 64, 128, 255}}
    });
}

