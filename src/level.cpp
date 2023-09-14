#include "../include/level.h"

Level::Level(std::shared_ptr<Player>& player, std::shared_ptr<TextureManager>& textureManager)
        : m_player(player), m_textureManager(textureManager)
{
    std::ifstream f("resources/level/forest_2.json");
    nlohmann::json data = nlohmann::json::parse(f);
    world = data["grid"];

    loadLevel();

    initialiseGameEntities();
}

// TODO MOVE TO ENGINE CLASS NOW THAT IT SIMPLY CHECKS KEYBOARD AND THEN CALLS A FUNC? 
void Level::update(GameState& gameState)
{
    map.update(gameState);

    for (auto& entity: gameEntities)
    {
        entity->update(gameState);
    }
}

void Level::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    map.draw(renderTarget, states);

    for (auto& entity: gameEntities)
    {
        entity->draw(renderTarget, sf::RenderStates::Default);
    }
}

void Level::enableEntityTileHighlightsForDebug(std::unordered_map<EntityType, sf::Color> entityTypeTileColour)
{
    for (const auto& entity: gameEntities)
    {
        sf::Color tileColour = entityTypeTileColour.at(entity->getType());
        map.highlightTileForDebug(entity, world.at(0).size(), tileColour);
    }
}

void Level::interactWithNode(sf::Time deltaTime)
{
    // TODO Create a tile object rather than a pair here?
    sf::Vector2u nextPlayerFacingTile = m_player->findNextTileDirection(deltaTime);
    uint8_t nodeFacingPlayer = world.at(nextPlayerFacingTile.y).at(nextPlayerFacingTile.x);

    if (nodeFacingPlayer == 2 || nodeFacingPlayer == 3)
    {
        world.at(nextPlayerFacingTile.y).at(nextPlayerFacingTile.x) = 0;

        // TODO not ideal, fix me
        loadLevel();
    }
}

void Level::loadLevel()
{
    if (!map.load("./resources/assets/basic_tilemap.png", sf::Vector2u(TILE_SIZE, TILE_SIZE), world))
    {
        std::cout << "Failed to load tileset" << std::endl;
        return;
    }
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
    std::cout << "Loading gameEntities from sprite sheet [rows: " << totalRows << ", cols: " << totalCols << "]\n";

    for (uint32_t rows = 0; rows < totalRows; rows++)
    {
        for (uint32_t cols = 0; cols < totalCols; cols++)
        {
            if ((rows * 4) + cols >= TOTAL_ENEMIES)
            { return; }

            uint32_t enemyRectLeft = ENEMY_WIDTH * (3 * cols);
            uint32_t enemyRectTop = ENEMY_HEIGHT * (4 * rows);

            // Note: These positions are temporary.
            auto enemyX = static_cast<float>(std::experimental::randint(TILE_SIZE, (TILE_SIZE - 1) * getWorldWidth()));
            auto enemyY = static_cast<float>(std::experimental::randint(TILE_SIZE, (TILE_SIZE - 1) * getWorldHeight()));

            sf::Vector2u spritePositionGroup = sf::Vector2u(enemyRectLeft, enemyRectTop);
            sf::Vector2f enemyPosition = sf::Vector2f(enemyX, enemyY);

            gameEntities.emplace_back(
                    std::make_shared<Enemy>(m_textureManager, m_player, enemyPosition, spritePositionGroup)
            );
        }
    }
}

// Important to return a copy here - TODO can we do this a different way?
std::vector<std::vector<uint32_t>> Level::getWorld()
{
    return world;
}

uint32_t Level::getWorldWidth()
{
    return world.at(0).size();
}

uint32_t Level::getWorldHeight()
{
    return world.size();
}