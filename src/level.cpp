#include "../include/level.h"

Level::Level(std::shared_ptr<Player>& player, std::shared_ptr<TextureManager>& textureManager)
        : m_player(player), m_textureManager(textureManager)
{
    initialiseGameEntities();
}

void Level::update(GameState& gameState)
{
    tileMap.update(gameState);

    for (auto& entity: gameEntities)
    {
        entity->update(gameState);
    }
}

void Level::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    tileMap.draw(renderTarget, states);

    for (auto& entity: gameEntities)
    {
        entity->draw(renderTarget, sf::RenderStates::Default);
    }
}

void Level::interactWithTile(sf::Time deltaTime, std::shared_ptr<Player>& player, Tile::Type tileType)
{
    sf::Vector2u nextPlayerFacingTilePosition = player->findNextTileFromDirection(deltaTime);
    Tile& nextPlayerFacingTile = TileMap::getTile(nextPlayerFacingTilePosition.x, nextPlayerFacingTilePosition.y);
    tileMap.updateTile(nextPlayerFacingTile, tileType);
}

void Level::initialiseGameEntities()
{
    gameEntities.clear();
    gameEntities.reserve(TOTAL_GAME_ENTITIES);
    gameEntities.push_back(m_player);

    // Load all characters on sprite sheet into memory.
    uint8_t rowCountByTotalEnemies = std::ceil(TOTAL_ENEMIES / TOTAL_ENTITIES_PER_SPRITE_SHEET_ROW);

    uint8_t minRows = 1;
    uint8_t totalRows = std::max(rowCountByTotalEnemies, minRows);
    uint8_t totalCols = totalRows == minRows
            ? TOTAL_ENEMIES
            : std::ceil(TOTAL_ENEMIES / totalRows);

    std::cout << "Loading gameEntities from sprite sheet ["
              << "rows: " << std::to_string(totalRows) << ", "
              << "cols: " << std::to_string(totalCols) << "]\n";

    for (uint32_t rows = 0; rows < totalRows; rows++)
    {
        for (uint32_t cols = 0; cols < totalCols; cols++)
        {
            if ((rows * 4) + cols >= TOTAL_ENEMIES)
            {
                return;
            }

            uint32_t enemyRectLeft = ENEMY_WIDTH * (3 * cols);
            uint32_t enemyRectTop = ENEMY_HEIGHT * (4 * rows);

            // Note: These positions are temporary.
            auto enemyX = static_cast<float>(std::experimental::randint(TILE_SIZE, (TILE_SIZE - 1) * TileMap::getWorldWidthInTiles()));
            auto enemyY = static_cast<float>(std::experimental::randint(TILE_SIZE, (TILE_SIZE - 1) * TileMap::getWorldHeightInTiles()));

            sf::Vector2u spritePositionGroup = sf::Vector2u(enemyRectLeft, enemyRectTop);
            sf::Vector2f enemyPosition = sf::Vector2f(enemyX, enemyY);

            gameEntities.emplace_back(
                    std::make_shared<Enemy>(m_textureManager, m_player, enemyPosition, spritePositionGroup)
            );
        }
    }
}

void Level::enableEntityTileHighlightsForDebug(std::unordered_map<EntityType, sf::Color> entityTypeTileColour)
{
    for (const auto& entity: gameEntities)
    {
        sf::Color tileColour = entityTypeTileColour.at(entity->getType());
        tileMap.highlightTileForDebug(entity, TileMap::getWorldWidthInTiles(), tileColour);
    }
}
