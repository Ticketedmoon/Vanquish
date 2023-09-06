#include "../include/level.h"


Level::Level(std::shared_ptr<Player>& player, std::vector<std::shared_ptr<GameEntity>>& entities)
    : player(player), entities(entities)
{
    std::ifstream f("resources/level/forest_2.json");
    nlohmann::json data = nlohmann::json::parse(f);
    this->world = data["grid"];
    loadLevel();
}

// TODO MOVE TO ENGINE CLASS NOW THAT IT SIMPLY CHECKS KEYBOARD AND THEN CALLS A FUNC? 
void Level::update(sf::Time& deltaTime)
{
    std::optional<uint32_t> spriteSheetTop;
    std::optional<uint32_t> spriteSheetLeft;

    // TODO/REFACTOR: Centralise these 4 blocks as they share similar characteristics.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        checkForPlayerMovement(deltaTime, EntityDirection::UP);
        spriteSheetTop = PLAYER_HEIGHT * 3;
        spriteSheetLeft = (player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        checkForPlayerMovement(deltaTime, EntityDirection::DOWN);
        spriteSheetTop = PLAYER_HEIGHT * 0;
        spriteSheetLeft = (player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        checkForPlayerMovement(deltaTime, EntityDirection::LEFT);
        spriteSheetTop = PLAYER_HEIGHT * 1;
        spriteSheetLeft = (player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        checkForPlayerMovement(deltaTime, EntityDirection::RIGHT);
        spriteSheetTop = PLAYER_HEIGHT * 2;
        spriteSheetLeft = (player->entitySpriteSheetDimRect.left == (PLAYER_WIDTH * 7)) ? 0 : (player->entitySpriteSheetDimRect.left + PLAYER_WIDTH);
    }

    if (spriteSheetTop.has_value() && spriteSheetLeft.has_value())
    {
        player->updateAnimation(deltaTime, spriteSheetTop.value(), spriteSheetLeft.value());
    }
}

void Level::interactWithNode(sf::Time& deltaTime)
{
    // TODO Create a tile object rather than a pair here?
    sf::Vector2u nextPlayerFacingTile = player->findNextTileFromPlayerDirection(deltaTime, player->getDirection());
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
    for (const auto& entity: entities) {
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
    player->setDirection(dir);

    // TODO Create a tile object rather than a pair here?
    sf::Vector2u nextPlayerFacingTile = player->findNextTileFromPlayerDirection(deltaTime, dir);

    if (world.at(nextPlayerFacingTile.y).at(nextPlayerFacingTile.x))
    {
        // Reset player velocity if they bump into an obstacle;
        player->velocity = sf::Vector2f(0, 0);
        return;
    }

    player->updatePosition(deltaTime, world.at(0).size(), world.size());
}

uint32_t Level::getLevelWidth()
{
    return this->world.at(0).size();
}

uint32_t Level::getLevelHeight()
{
    return this->world.size();
}

TileMap Level::getMap()
{
    return map;
}
