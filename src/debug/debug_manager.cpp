#include "debug/debug_manager.h"

DebugManager::DebugManager(std::shared_ptr<Player>& player, Level& level, std::shared_ptr<TextManager>& textManager)
    : player(player), level(level), textManager(textManager)
{

}

void DebugManager::render(sf::RenderTarget& renderTarget, sf::RenderStates states, GameState gameState)
{
    renderTarget.setView(renderTarget.getDefaultView());
    startDebugView();
}

void DebugManager::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{

}

/*
 * TODO ADD IMPROVEMENT TO ENABLE DEBUG CODE ONLY IF CODE IS RUN IN DEBUG CONFIGURATION.
 *      WE DO NOT WANT TO HAVE DEBUG CODE IN DISTRIBUTION BUILDS.
 * TODO This has nothing to do with an sf::view, so maybe move to a DebugManager or in RenderingManager
 */
void DebugManager::startDebugView()
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

    textManager->drawText(text, sf::Color::Green, 12, sf::Vector2f(25, 50));

    level.enableEntityTileHighlightsForDebug({
        {EntityType::PLAYER, sf::Color{0, 196, 128, 255}},
        {EntityType::ENEMY, sf::Color{255, 64, 128, 255}}
    });
}
