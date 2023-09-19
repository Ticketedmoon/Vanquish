#include "debug/debug_manager.h"

DebugManager::DebugManager(std::shared_ptr<Player>& player, std::vector<std::shared_ptr<GameEntity>> entities,
        Level& level, std::shared_ptr<TextManager>& textManager)
        : player(player), entities(std::move(std::move(entities))), level(level), textManager(textManager)
{
}

void DebugManager::update(GameState& gameState)
{
    std::string fps = std::to_string(1.0f / debugClock.restart().asSeconds());
    const std::string& playerPosX = std::to_string(player->getPosition().x);
    const std::string& playerPosY = std::to_string(player->getPosition().y);
    const std::string& playerTileX = std::to_string(player->tilePosition.x);
    const std::string& playerTileY = std::to_string(player->tilePosition.y);
    const std::string& worldTimeSeconds = std::to_string(gameState.getClock().getWorldTimeSeconds());

    debugTextValue =
            "fps: " + fps + "\n" +
            "window position (x, y): " + "(" + playerPosX + ", " + playerPosY + ")" + "\n" +
            "tile position (x, y): " + "(" + playerTileX + ", " + playerTileY + ")" + "\n" +
            "Game World Time (seconds): " + "(" + worldTimeSeconds + ")" + "\n";
}

void DebugManager::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    for (const std::shared_ptr<GameEntity>& entity: entities)
    {
        drawBoundingBoxAroundEntity(renderTarget, states, entity);
    }

    drawBoundingBoxAroundEntity(renderTarget, states, player);

    renderTarget.setView(renderTarget.getDefaultView());
    textManager->drawText(debugTextValue, sf::Color::Green, 12, sf::Vector2f(25, 100));

    level.enableEntityTileHighlightsForDebug({
            {EntityType::PLAYER, sf::Color{0, 196, 128, 255}},
            {EntityType::ENEMY,  sf::Color{255, 64, 128, 255}}
    });
}

void DebugManager::drawBoundingBoxAroundEntity(sf::RenderTarget& renderTarget, const sf::RenderStates& states,
        const std::shared_ptr<GameEntity>& entity) const
{
    const sf::Vector2<float>& boundingBoxDimensions = sf::Vector2f(entity->getWidth(), entity->getHeight());
    sf::RectangleShape entityBoundingBox = sf::RectangleShape(boundingBoxDimensions);
    entityBoundingBox.setPosition(entity->getPosition());
    entityBoundingBox.setFillColor(sf::Color::Transparent);
    entityBoundingBox.setOutlineColor(sf::Color::Black);
    entityBoundingBox.setOutlineThickness(1.0f);
    renderTarget.draw(entityBoundingBox, states);
}

void DebugManager::updateDebugState(GameState& gameState)
{
    if (gameState.getState() == GameState::State::PLAYING)
    {
        gameState.updateGameState(GameState::State::DEBUG);
    }
    else
    {
        gameState.updateGameState(GameState::State::PLAYING);
        // Clear tiles of highlighting
        level.enableEntityTileHighlightsForDebug({
                {EntityType::PLAYER, sf::Color::White},
                {EntityType::ENEMY,  sf::Color::White}
        });
    }
}
