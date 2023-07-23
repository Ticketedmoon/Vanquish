#include "../include/level.h"
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <vector>

Level::Level(Player* player)
{
    for (uint32_t i = 0; i < TOTAL_COLS_IN_LEVEL; i++)
    {
        std::vector<uint32_t> row;
        for (uint32_t j = 0; j < TOTAL_ROWS_IN_LEVEL; j++)
        {
            row.emplace_back(0);
        }

        // DEBUG
        row.at(TOTAL_ROWS_IN_LEVEL/2) = 2;

        this->world.emplace_back(row);
    }

    loadLevel();
    this->player = player;
}

void Level::update(sf::Clock& clock)
{
    checkForMoveVerticalDirection(clock, player->tilePosition.x, player->tilePosition.y);
    checkForMoveHorizontalDirection(clock, player->tilePosition.x, player->tilePosition.y);
    checkForChopTree(player->tilePosition.x, player->tilePosition.y);
}

void Level::loadLevel()
{
    if (!map.load("./resources/assets/basic_tilemap.png", sf::Vector2u(32, 32), world, TOTAL_ROWS_IN_LEVEL, TOTAL_COLS_IN_LEVEL))
    {
        std::cout << "Failed to load tileset" << std::endl;
        return;
    }
}

void Level::checkForMoveVerticalDirection(sf::Clock& clock, uint32_t tileUnderPlayerX, uint32_t tileUnderPlayerY)
{
    // TODO Move keyboard check to engine/controller class
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        player->updateAnimation(clock, 0, PlayerDirection::DOWN);
        uint32_t nextTileY = tileUnderPlayerY+1 >= world.size() ? tileUnderPlayerY : tileUnderPlayerY+1;

        if (world.at(nextTileY).at(tileUnderPlayerX) > 0)
        {
            return;
        }

        player->updatePlayerPosition(world.at(0).size(), world.size());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player->updateAnimation(clock, 3, PlayerDirection::UP);
        uint32_t nextTileY = tileUnderPlayerY > 0 ? tileUnderPlayerY-1 : 0;

        if (world.at(nextTileY).at(tileUnderPlayerX) > 0)
        {
            return;
        }

        player->updatePlayerPosition(world.at(0).size(), world.size());
    }

}


void Level::checkForMoveHorizontalDirection(sf::Clock& clock, uint32_t tileUnderPlayerX, uint32_t tileUnderPlayerY)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player->updateAnimation(clock, 1, PlayerDirection::LEFT);
        uint32_t nextTileX = (tileUnderPlayerX > 0) ? tileUnderPlayerX-1 : 0;

        if (world.at(tileUnderPlayerY).at(nextTileX) > 0)
        {
            return;
        }

        player->updatePlayerPosition(world.at(0).size(), world.size());
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player->updateAnimation(clock, 2, PlayerDirection::RIGHT);
        uint32_t nextTileX = tileUnderPlayerX < world.at(0).size()-1 ? tileUnderPlayerX+1 : tileUnderPlayerX;

        if (world.at(tileUnderPlayerY).at(tileUnderPlayerX) > 0)
        {
            return;
        }

        player->updatePlayerPosition(world.at(0).size(), world.size());
    }
}

// TODO centralise common logic in this method
void Level::checkForChopTree(uint32_t tileUnderPlayerX, uint32_t tileUnderPlayerY)
{
    sf::Vector2f playerPos = player->getPlayerPos();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (player->getPlayerDir() == PlayerDirection::UP)
        {
            // TODO Rename tileUnder* as it's confusing
            int nextTileY = tileUnderPlayerY > 0 ? tileUnderPlayerY-1 : tileUnderPlayerY;

            if (world.at(nextTileY).at(tileUnderPlayerX) == 2)
            {
                std::cout << "cut tree!" << std::endl;
                world.at(nextTileY).at(tileUnderPlayerX) = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::DOWN)
        {
            uint32_t nextTileY = tileUnderPlayerY < world.size() ? tileUnderPlayerY+1 : tileUnderPlayerY;
            if (world.at(nextTileY).at(tileUnderPlayerX) == 2)
            {
                std::cout << "cut tree!" << std::endl;
                world.at(nextTileY).at(tileUnderPlayerX) = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::LEFT)
        {
            uint32_t nextTileX = tileUnderPlayerX > 0 ? tileUnderPlayerX-1 : tileUnderPlayerX;
            if (world.at(tileUnderPlayerY).at(nextTileX) == 2)

            if (world.at(tileUnderPlayerY).at(nextTileX) == 2)
            {
                std::cout << "cut tree!" << std::endl;
                world.at(tileUnderPlayerY).at(nextTileX) = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::RIGHT)
        {
            uint32_t nextTileX = tileUnderPlayerX < world.at(0).size() ? tileUnderPlayerX+1 : tileUnderPlayerX;
            if (world.at(tileUnderPlayerY).at(nextTileX) == 2)
            {
                std::cout << "cut tree!" << std::endl;
                world.at(tileUnderPlayerY).at(nextTileX) = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }
    }
}

TileMap Level::getTileMap()
{
    return map;
}
