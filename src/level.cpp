#include "../include/level.h"
#include <cstdint>

// TODO Investigate if getters are conventional in c++

Level::Level(Player* player)
{
    std::ifstream f("resources/level/forest_0.json");
    nlohmann::json data = nlohmann::json::parse(f);
    this->world = data["grid"];
    loadLevel();
    this->player = player;
}

// TODO MOVE TO ENGINE CLASS NOW THAT IT SIMPLY CHECKS KEYBOARD AND THEN CALLS A FUNC? 
void Level::update(sf::Clock& clock)
{
    // Cardinal movement directions
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        checkForPlayerMovement(clock, PlayerDirection::UP, 3);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        checkForPlayerMovement(clock, PlayerDirection::DOWN, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        checkForPlayerMovement(clock, PlayerDirection::LEFT, 1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        checkForPlayerMovement(clock, PlayerDirection::RIGHT, 2);
    }

}

void Level::debug()
{
    // Move to render function
    map.highlightPlayerTile(player->tilePosition.x, player->tilePosition.y, world.at(0).size());
}

void Level::loadLevel()
{
    if (!map.load("./resources/assets/basic_tilemap.png", sf::Vector2u(32, 32), world))
    {
        std::cout << "Failed to load tileset" << std::endl;
        return;
    }
}

void Level::checkForPlayerMovement(sf::Clock& clock, PlayerDirection dir, uint32_t spriteOffset)
{
    player->updateAnimation(clock, spriteOffset, dir);

    // TODO Create a tile object rather than a pair here?
    std::pair<uint32_t, uint32_t> nextPlayerFacingTile = player->findNextTileFromPlayerDirection(dir);

    if (world.at(nextPlayerFacingTile.second).at(nextPlayerFacingTile.first))
    {
        // Reset player velocity if they bump into an obstacle;
        player->velocity = sf::Vector2f(0, 0);
        return;
    }

    player->updatePlayerPosition(world.at(0).size(), world.size());
}


void Level::interactWithNode()
{
    // TODO Create a tile object rather than a pair here?
    std::pair<uint32_t, uint32_t> nextPlayerFacingTile = player->findNextTileFromPlayerDirection(player->getPlayerDir());
    uint8_t nodeFacingPlayer = world.at(nextPlayerFacingTile.second).at(nextPlayerFacingTile.first);

    if (nodeFacingPlayer == 2 || nodeFacingPlayer == 3)
    {
        world.at(nextPlayerFacingTile.second).at(nextPlayerFacingTile.first) = 0;

        // TODO not ideal, fix me
        loadLevel();
    }
}
