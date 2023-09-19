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

        uint32_t damageDistanceThreshold = TILE_SIZE;
        if (std::abs(entity->getPosition().y - m_player->getPosition().y) < damageDistanceThreshold
            && std::abs(entity->getPosition().x - m_player->getPosition().x) < damageDistanceThreshold)
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

// FIXME: This logic simply just updates the position of the target toward the direction the player faces.
//        This is not satisfying and instead we want to see the velocity and acceleration of the pushback.
void Level::tryDamageEnemy(GameState& gameState, std::shared_ptr<GameEntity>& entity,
        std::pair<EntityDirection, EntityDirection> facingDirections)
{
    EntityDirection enemyDirection = facingDirections.first;
    EntityDirection playerDirection = facingDirections.second;
    if (entity->getDirection() == enemyDirection && m_player->getDirection() == playerDirection)
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

            entity->lastTimeTakenDamageSeconds = gameState.getClock().getWorldTimeSeconds() + 0.5;
        }

        // Perform pushback
        // NOTE: If the pushback amount is small, the enemy will stay in the range of the blade and take repeated damage.
        //       This could be a useful 'skill' or type of attack in the future.
        int pushbackAmount = 80;
        sf::Vector2<float> newPositionAfterPushback = playerDirection == EntityDirection::UP
                ? sf::Vector2f(entity->getPosition().x, entity->getPosition().y - pushbackAmount)
                : playerDirection == EntityDirection::DOWN
                        ? sf::Vector2f(entity->getPosition().x, entity->getPosition().y + pushbackAmount)
                        : playerDirection == EntityDirection::LEFT
                                ? sf::Vector2f(entity->getPosition().x - pushbackAmount, entity->getPosition().y)
                                : sf::Vector2f(entity->getPosition().x + pushbackAmount, entity->getPosition().y);

        entity->setPosition(newPositionAfterPushback);
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
            auto enemyX = static_cast<float>(std::experimental::randint(TILE_SIZE,
                    (TILE_SIZE - 1) * TileMap::getWorldWidthInTiles()));
            auto enemyY = static_cast<float>(std::experimental::randint(TILE_SIZE,
                    (TILE_SIZE - 1) * TileMap::getWorldHeightInTiles()));

            sf::Vector2u spritePositionGroup = sf::Vector2u(enemyRectLeft, enemyRectTop);
            sf::Vector2f enemyPosition = sf::Vector2f(enemyX, enemyY);

            uint16_t damage = std::experimental::randint(5, 20);
            int enemyXp = damage * 12;
            uint32_t enemyId = ((TOTAL_ENTITIES_PER_SPRITE_SHEET_ROW * rows) + cols + 1);

            gameEntities.emplace_back(
                    std::make_shared<Enemy>(enemyId,
                            m_textureManager,
                            m_player,
                            enemyPosition,
                            spritePositionGroup,
                            damage,
                            enemyXp)
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

std::vector<std::shared_ptr<GameEntity>> Level::getEnemies()
{
    std::vector<std::shared_ptr<GameEntity>> enemies;
    for (const auto& entity: gameEntities)
    {
        if (entity->getType() == EntityType::ENEMY)
        {
            enemies.emplace_back(entity);
        }
    }
    return enemies;
}
