#include "../include/level.h"

Level::Level(std::shared_ptr<Player>& player, std::vector<std::shared_ptr<GameEntity>>& entities) : player(player), entities(entities)
{
    std::ifstream f("resources/level/forest_2.json");
    nlohmann::json data = nlohmann::json::parse(f);
    this->world = data["grid"];
    loadLevel();
}

// TODO MOVE TO ENGINE CLASS NOW THAT IT SIMPLY CHECKS KEYBOARD AND THEN CALLS A FUNC? 
void Level::update(sf::Time& deltaTime, sf::Clock& worldClock)
{
    // TODO REFACTOR
    // Cardinal movement directions
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        checkForPlayerMovement(deltaTime, worldClock, EntityDirection::UP, 3);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        checkForPlayerMovement(deltaTime, worldClock, EntityDirection::DOWN, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        checkForPlayerMovement(deltaTime, worldClock, EntityDirection::LEFT, 1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        checkForPlayerMovement(deltaTime, worldClock, EntityDirection::RIGHT, 2);
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

void Level::debug(bool shouldClear)
{
    // Move to render function
    if (shouldClear)
    {
        map.highlightTileForDebug(player, world.at(0).size(), sf::Color::White);
        for (const auto& entity : entities)
        {
            if (entity->getType() != EntityType::PLAYER)
            {
                map.highlightTileForDebug(entity, world.at(0).size(), sf::Color::White);

            }
        }
    }
    else
    {
        map.highlightTileForDebug(player, world.at(0).size(), sf::Color{0, 196, 128, 255});
        for (const auto& entity : entities)
        {
            if (entity->getType() != EntityType::PLAYER)
            {
                map.highlightTileForDebug(entity, world.at(0).size(), sf::Color{255, 64, 128, 255});
            }
        }
    }
}

void Level::loadLevel()
{
    if (!map.load("./resources/assets/basic_tilemap.png", sf::Vector2u(32, 32), world))
    {
        std::cout << "Failed to load tileset" << std::endl;
        return;
    }
}

void Level::checkForPlayerMovement(sf::Time& deltaTime, sf::Clock& worldClock, EntityDirection dir, uint32_t spriteOffset)
{
    uint32_t spriteSheetTop = PLAYER_HEIGHT * spriteOffset;
    uint32_t spriteSheetLeft = (player->entityDimRect.left == (PLAYER_WIDTH * 2)) ? 0 : (player->entityDimRect.left + PLAYER_WIDTH);
    player->updateAnimation(worldClock, spriteSheetTop, spriteSheetLeft);
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