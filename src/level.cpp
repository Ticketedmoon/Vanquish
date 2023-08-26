#include "../include/level.h"

Level::Level(std::shared_ptr<Player>& player) : player(player)
{
    std::ifstream f("resources/level/forest_2.json");
    nlohmann::json data = nlohmann::json::parse(f);
    this->world = data["grid"];
    loadLevel();
}

// TODO MOVE TO ENGINE CLASS NOW THAT IT SIMPLY CHECKS KEYBOARD AND THEN CALLS A FUNC? 
void Level::update(sf::Time& deltaTime, sf::Clock& worldClock)
{
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
    std::pair<uint32_t, uint32_t> nextPlayerFacingTile = player->findNextTileFromPlayerDirection(deltaTime, player->getDirection());
    uint8_t nodeFacingPlayer = world.at(nextPlayerFacingTile.second).at(nextPlayerFacingTile.first);

    if (nodeFacingPlayer == 2 || nodeFacingPlayer == 3)
    {
        world.at(nextPlayerFacingTile.second).at(nextPlayerFacingTile.first) = 0;

        // TODO not ideal, fix me
        loadLevel();
    }
}

void Level::debug(bool shouldClear)
{
    // Move to render function
    if (shouldClear)
    {
        map.highlightPlayerTile(player->tilePosition.x, player->tilePosition.y, world.at(0).size(), sf::Color::White);
    }
    else
    {
        map.highlightPlayerTile(player->tilePosition.x, player->tilePosition.y, world.at(0).size(), sf::Color::Magenta);
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
    std::pair<uint32_t, uint32_t> nextPlayerFacingTile = player->findNextTileFromPlayerDirection(deltaTime, dir);

    if (world.at(nextPlayerFacingTile.second).at(nextPlayerFacingTile.first))
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