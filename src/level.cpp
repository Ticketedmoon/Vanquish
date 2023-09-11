#include "../include/level.h"
#include "engine.h"

Level::Level(std::shared_ptr<Player>& player, std::shared_ptr<TextureManager>& textureManager) : m_player(player), m_textureManager(textureManager)
{
    std::ifstream f("resources/level/forest_2.json");
    nlohmann::json data = nlohmann::json::parse(f);
    this->world = data["grid"];

    loadLevel();

    initialiseGameEntities();
}

// TODO MOVE TO ENGINE CLASS NOW THAT IT SIMPLY CHECKS KEYBOARD AND THEN CALLS A FUNC? 
void Level::update(sf::Clock& worldClock, sf::Time& deltaTime, uint32_t levelWidth, uint32_t levelHeight)
{
    for (auto& entity : gameEntities)
    {
        entity->update(worldClock, deltaTime, levelWidth, levelHeight);
    }

    std::optional<uint32_t> spriteSheetTop;
    std::optional<uint32_t> spriteSheetLeft;

    // TODO/REFACTOR: Centralise these 4 blocks as they share similar characteristics.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        checkForPlayerMovement(deltaTime, EntityDirection::UP);
        spriteSheetTop = PLAYER_HEIGHT * 3;
        spriteSheetLeft = (m_player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (m_player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        checkForPlayerMovement(deltaTime, EntityDirection::DOWN);
        spriteSheetTop = PLAYER_HEIGHT * 0;
        spriteSheetLeft = (m_player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (m_player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        checkForPlayerMovement(deltaTime, EntityDirection::LEFT);
        spriteSheetTop = PLAYER_HEIGHT * 1;
        spriteSheetLeft = (m_player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (m_player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        checkForPlayerMovement(deltaTime, EntityDirection::RIGHT);
        spriteSheetTop = PLAYER_HEIGHT * 2;
        spriteSheetLeft = (m_player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (m_player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (spriteSheetTop.has_value() && spriteSheetLeft.has_value())
    {
        m_player->updateAnimation(deltaTime, spriteSheetTop.value(), spriteSheetLeft.value());
    }
}

void Level::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    renderTarget.draw(map);

    for (auto& entity : gameEntities)
    {
        entity->draw(renderTarget, sf::RenderStates::Default);
    }
}

void Level::interactWithNode(sf::Time& deltaTime)
{
    // TODO Create a tile object rather than a pair here?
    sf::Vector2u nextPlayerFacingTile = m_player->findNextTileFromPlayerDirection(deltaTime, m_player->getDirection());
    uint8_t nodeFacingPlayer = world.at(nextPlayerFacingTile.y).at(nextPlayerFacingTile.x);

    if (nodeFacingPlayer == 2 || nodeFacingPlayer == 3)
    {
        world.at(nextPlayerFacingTile.y).at(nextPlayerFacingTile.x) = 0;

        // TODO not ideal, fix me
        loadLevel();
    }
}

void Level::enableEntityTileHighlightsForDebug(std::unordered_map<EntityType, sf::Color> entityTypeTileColour)
{
    for (const auto& entity: gameEntities) {
        sf::Color tileColour = entityTypeTileColour.at(entity->getType());
        map.highlightTileForDebug(entity, world.at(0).size(), tileColour);
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

void Level::checkForPlayerMovement(sf::Time& deltaTime, EntityDirection dir)
{
    m_player->setDirection(dir);

    // TODO Create a tile object rather than a pair here?
    sf::Vector2u nextPlayerFacingTile = m_player->findNextTileFromPlayerDirection(deltaTime, dir);

    if (world.at(nextPlayerFacingTile.y).at(nextPlayerFacingTile.x))
    {
        // Reset player velocity if they bump into an obstacle;
        m_player->velocity = sf::Vector2f(0, 0);
        return;
    }

    m_player->updatePosition(deltaTime, world.at(0).size(), world.size());
}

void Level::initialiseGameEntities()
{
    gameEntities.clear();
    gameEntities.reserve(TOTAL_GAME_ENTITIES);
    gameEntities.push_back(m_player);

    // Load all characters on sprite sheet into memory.
    double rowCountByTotalEnemies = ceil(TOTAL_ENEMIES / 4.0f);
    size_t totalRows = std::max(rowCountByTotalEnemies, static_cast<double>(1));
    size_t totalCols = totalRows == 1 ? TOTAL_ENEMIES : ceil(TOTAL_ENEMIES/2.0);
    std::cout << "Loading gameEntities from sprite sheet [rows: " << totalRows << ", cols: " << totalCols << "]\n";

    for (uint32_t rows = 0; rows < totalRows; rows++) {
        for (uint32_t cols = 0; cols < totalCols; cols++) {
            if ((rows*4) + cols >= TOTAL_ENEMIES) return;

            uint32_t enemyRectLeft = ENEMY_WIDTH * (3 * cols);
            uint32_t enemyRectTop = ENEMY_HEIGHT * (4 * rows);

            // Note: These positions are temporary.
            uint32_t enemyX = std::experimental::randint(TILE_SIZE, (TILE_SIZE-1) * getLevelWidth());
            uint32_t enemyY = std::experimental::randint(TILE_SIZE, (TILE_SIZE-1) * getLevelHeight());

            gameEntities.emplace_back(
                    std::make_shared<Enemy>(m_textureManager, m_player, enemyX, enemyY, enemyRectLeft, enemyRectTop));
        }
    }
}


uint32_t Level::getLevelWidth()
{
    return this->world.at(0).size();
}

uint32_t Level::getLevelHeight()
{
    return this->world.size();
}