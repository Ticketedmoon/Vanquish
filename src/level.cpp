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

    if (gameState.getPlayerState() != GameState::PlayerState::ATTACK)
    {
        return;
    }

    checkForPlayerAttackOnEnemy(gameState);

}

void Level::checkForPlayerAttackOnEnemy(GameState& gameState)
{
    for (auto& entity: gameEntities)
    {
        // Note: This check is a little silly. If the polymorphism is forcing these checks, maybe it's worth splitting
        //       out the draw calls.
        if (entity->getType() != EntityType::ENEMY)
        {
            continue;
        }

        if (std::abs(entity->getPosition().y - m_player->getPosition().y) < TILE_SIZE
            && std::abs(entity->getPosition().x - m_player->getPosition().x) < TILE_SIZE)
        {
            tryDamageEnemy(gameState, entity, std::make_pair(EntityDirection::LEFT, EntityDirection::RIGHT));
            tryDamageEnemy(gameState, entity, std::make_pair(EntityDirection::RIGHT, EntityDirection::LEFT));
            tryDamageEnemy(gameState, entity, std::make_pair(EntityDirection::UP, EntityDirection::DOWN));
            tryDamageEnemy(gameState, entity, std::make_pair(EntityDirection::DOWN, EntityDirection::UP));
        }
    }

    auto it = std::remove_if(gameEntities.begin(), gameEntities.end(),
            [](std::shared_ptr<GameEntity> entity) {
                return entity->isDead();
            });
    gameEntities.erase(it, gameEntities.end());
}

void Level::draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const
{
    tileMap.draw(renderTarget, states);

    for (auto& entity: gameEntities)
    {
        entity->draw(renderTarget, states);
    }
}

void Level::interactWithTile(sf::Time deltaTime, std::shared_ptr<Player>& player, Tile::Type tileType)
{
    sf::Vector2u nextPlayerFacingTilePosition = player->findNextTileFromDirection(deltaTime);
    Tile& nextPlayerFacingTile = TileMap::getTile(nextPlayerFacingTilePosition.x, nextPlayerFacingTilePosition.y);
    tileMap.updateTile(nextPlayerFacingTile, tileType);
}

void Level::tryDamageEnemy(GameState& gameState, std::shared_ptr<GameEntity>& entity,
        std::pair<EntityDirection, EntityDirection> facingDirections)
{
    if (entity->getDirection() == facingDirections.first && m_player->getDirection() == facingDirections.second)
    {
        // damage enemy
        if (gameState.getClock().getWorldTimeSeconds() > entity->lastTimeTakenDamageSeconds)
        {
            entity->updateSpriteColour(sf::Color::Red);
            m_player->damageTarget(gameState.getClock(), entity);
            if (entity->isDead())
            {
                m_player->addExperiencePoints(entity);
            }
            entity->lastTimeTakenDamageSeconds = gameState.getClock().getWorldTimeSeconds() + 0.25;
        }

        // perform pushback
        // temporary
        entity->setPosition(sf::Vector2f(entity->getPosition() - sf::Vector2f(80, 80)));
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

            uint16_t damage = std::experimental::randint(5, 20);
            int enemyXp = damage * 12;

            gameEntities.emplace_back(
                    std::make_shared<Enemy>(m_textureManager, m_player, enemyPosition, spritePositionGroup, damage, enemyXp)
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
