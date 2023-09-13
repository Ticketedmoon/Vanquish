#include "../include/level.h"

Level::Level(std::shared_ptr<Player>& player, std::shared_ptr<TextureManager>& textureManager)
    : m_player(player), m_textureManager(textureManager)
{
    std::ifstream f("resources/level/forest_2.json");
    nlohmann::json data = nlohmann::json::parse(f);
    this->world = data["grid"];

    loadLevel();

    initialiseGameEntities();
}

// TODO MOVE TO ENGINE CLASS NOW THAT IT SIMPLY CHECKS KEYBOARD AND THEN CALLS A FUNC? 
void Level::update(GameState& gameState)
{
    for (auto& entity : gameEntities)
    {
        entity->update(gameState);
    }

    std::optional<uint32_t> spriteSheetTop;
    std::optional<uint32_t> spriteSheetLeft;

    GameClock& gameClock = gameState.getClock();

    // TODO/REFACTOR: Centralise these 4 blocks as they share similar characteristics.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        checkForPlayerMovement(gameClock, EntityDirection::UP);
        spriteSheetTop = PLAYER_HEIGHT * 3;
        spriteSheetLeft = (m_player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (m_player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        checkForPlayerMovement(gameClock, EntityDirection::DOWN);
        spriteSheetTop = PLAYER_HEIGHT * 0;
        spriteSheetLeft = (m_player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (m_player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        checkForPlayerMovement(gameClock, EntityDirection::LEFT);
        spriteSheetTop = PLAYER_HEIGHT * 1;
        spriteSheetLeft = (m_player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (m_player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        checkForPlayerMovement(gameClock, EntityDirection::RIGHT);
        spriteSheetTop = PLAYER_HEIGHT * 2;
        spriteSheetLeft = (m_player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (m_player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (spriteSheetTop.has_value() && spriteSheetLeft.has_value())
    {
        m_player->updateAnimation(gameClock.getDeltaTime(), spriteSheetTop.value(), spriteSheetLeft.value());
    }
}

void Level::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    map.draw(renderTarget, states);

    for (auto& entity : gameEntities)
    {
        entity->draw(renderTarget, sf::RenderStates::Default);
    }
}

void Level::interactWithNode(sf::Time deltaTime)
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

void Level::checkForPlayerMovement(GameClock& gameClock, EntityDirection dir)
{
    m_player->setDirection(dir);

    // TODO Create a tile object rather than a pair here?
    sf::Vector2u nextPlayerFacingTile = m_player->findNextTileFromPlayerDirection(gameClock.getDeltaTime(), dir);

    if (world.at(nextPlayerFacingTile.y).at(nextPlayerFacingTile.x))
    {
        // Reset player velocity if they bump into an obstacle;
        m_player->velocity = sf::Vector2f(0, 0);
        return;
    }

    m_player->updatePosition(gameClock.getDeltaTime(), getWorldWidth(), getWorldHeight());
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

    const sf::Vector2u levelDimensions = sf::Vector2u(getWorldWidth(), getWorldHeight());

    for (uint32_t rows = 0; rows < totalRows; rows++) {
        for (uint32_t cols = 0; cols < totalCols; cols++) {
            if ((rows*4) + cols >= TOTAL_ENEMIES) return;

            uint32_t enemyRectLeft = ENEMY_WIDTH * (3 * cols);
            uint32_t enemyRectTop = ENEMY_HEIGHT * (4 * rows);

            // Note: These positions are temporary.
            float enemyX = std::experimental::randint(TILE_SIZE, (TILE_SIZE-1) * getWorldWidth());
            float enemyY = std::experimental::randint(TILE_SIZE, (TILE_SIZE-1) * getWorldHeight());

            sf::Vector2u spritePositionGroup = sf::Vector2u(enemyRectLeft, enemyRectTop);
            sf::Vector2f enemyPosition = sf::Vector2f(enemyX, enemyY);

            gameEntities.emplace_back(
                    std::make_shared<Enemy>(m_textureManager, m_player, enemyPosition, spritePositionGroup,
                                            levelDimensions));
        }
    }
}


uint32_t Level::getWorldWidth()
{
    return this->world.at(0).size();
}

uint32_t Level::getWorldHeight()
{
    return this->world.size();
}