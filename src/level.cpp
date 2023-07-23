#include "../include/level.h"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <vector>

Level::Level(Player* player, uint32_t totalRowsForLevel, uint32_t totalColsForLevel)
{
    for (uint32_t i = 0; i < totalColsForLevel; i++)
    {
        std::vector<uint32_t> row;
        for (uint32_t j = 0; j < totalRowsForLevel; j++)
        {
            row.emplace_back(0);
        }

        // DEBUG
        row.at(totalRowsForLevel/2) = 2;

        this->world.emplace_back(row);
    }

    loadLevel();
    this->player = player;
}

void Level::update(sf::Clock& clock)
{
    // Cardinal movement directions
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        uint32_t nextTileY = player->tilePosition.y > 0 ? player->tilePosition.y-1 : 0;
        checkForPlayerMovement(clock, PlayerDirection::UP, player->tilePosition.x, nextTileY, 3);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        uint32_t nextTileY = player->tilePosition.y+1 >= world.size() ? player->tilePosition.y : player->tilePosition.y+1;
        checkForPlayerMovement(clock, PlayerDirection::DOWN, player->tilePosition.x, nextTileY, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        uint32_t nextTileX = (player->tilePosition.x > 0) ? player->tilePosition.x-1 : 0;
        checkForPlayerMovement(clock, PlayerDirection::LEFT, nextTileX, player->tilePosition.y, 1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        uint32_t nextTileX = player->tilePosition.x < world.at(0).size()-1 ? player->tilePosition.x+1 : player->tilePosition.x;
        checkForPlayerMovement(clock, PlayerDirection::RIGHT, nextTileX, player->tilePosition.y, 2);
    }
}

void Level::loadLevel()
{
    if (!map.load("./resources/assets/basic_tilemap.png", sf::Vector2u(32, 32), world, world.size(), world.at(0).size()))
    {
        std::cout << "Failed to load tileset" << std::endl;
        return;
    }
}

void Level::checkForPlayerMovement(sf::Clock& clock, PlayerDirection dir, uint32_t tileX, uint32_t tileY, uint32_t spriteOffset)
{
    player->updateAnimation(clock, spriteOffset, dir);

    if (world.at(tileY).at(tileX) > 0)
    {
        return;
    }

    player->updatePlayerPosition(world.at(0).size(), world.size());
}


void Level::chopTree()
{
    // up
    uint32_t nextTileY = player->tilePosition.y > 0 ? player->tilePosition.y-1 : player->tilePosition.y;
    chopTreeForPlayerDirection(PlayerDirection::UP, player->tilePosition.x, nextTileY);
    // down
    nextTileY = player->tilePosition.y < world.size()-1 ? player->tilePosition.y+1 : player->tilePosition.y;
    chopTreeForPlayerDirection(PlayerDirection::DOWN, player->tilePosition.x, nextTileY);
    // left
    uint32_t nextTileX = player->tilePosition.x > 0 ? player->tilePosition.x-1 : player->tilePosition.x;
    chopTreeForPlayerDirection(PlayerDirection::LEFT, nextTileX, player->tilePosition.y);
    // right
    nextTileX = player->tilePosition.x < world.at(0).size()-1 ? player->tilePosition.x+1 : player->tilePosition.x;
    chopTreeForPlayerDirection(PlayerDirection::RIGHT, nextTileX, player->tilePosition.y);
}

void Level::chopTreeForPlayerDirection(PlayerDirection dir, uint32_t tileX, uint32_t tileY)
{
    if (player->getPlayerDir() == dir)
    {
        if (world.at(tileY).at(tileX) == 2)
        {
            std::cout << "cut tree!" << std::endl;
            world.at(tileY).at(tileX) = 0;

            // TODO not ideal, fix me
            loadLevel();
        }
    }
}

TileMap Level::getTileMap()
{
    return map;
}
