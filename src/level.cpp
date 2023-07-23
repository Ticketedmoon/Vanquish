#include "../include/level.h"
#include <cstdint>
#include <vector>

Level::Level(Player* player)
{
    std::cout << "Building level..." << std::endl;
    for (uint32_t i = 0; i < 10; i++)
    {
        std::vector<uint32_t> row;
        for (uint32_t j = 0; j < 10; j++)
        {
            row.emplace_back(0);
        }
        this->world.emplace_back(row);
    }

    std::cout << "Level Height: " << this->world.size() << std::endl;
    std::cout << "Level Width: " << this->world.at(0).size() << std::endl;

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
    if (!map.load("./resources/assets/basic_tilemap.png", sf::Vector2u(32, 32), world))
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

        if (world.at(tileUnderPlayerX).at(nextTileY) > 0)
        {
            return;
        }

        player->updatePlayerPosition(world.at(0).size(), world.size());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player->updateAnimation(clock, 3, PlayerDirection::UP);
        uint32_t nextTileY = tileUnderPlayerY > 0 ? tileUnderPlayerY-1 : 0;

        if (world.at(tileUnderPlayerX).at(nextTileY) > 0)
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

        if (world.at(nextTileX).at(tileUnderPlayerY) > 0)
        {
            return;
        }

        player->updatePlayerPosition(world.at(0).size(), world.size());
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player->updateAnimation(clock, 2, PlayerDirection::RIGHT);

        uint32_t nextTileX = tileUnderPlayerX < world.at(0).size()-1 ? tileUnderPlayerX+1 : tileUnderPlayerX;
        if (world.at(nextTileX).at(tileUnderPlayerY) > 0)
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
            int nextTileY = tileUnderPlayerY > 0 ? tileUnderPlayerY-1 : 0;

            if (world.at(tileUnderPlayerX).at(nextTileY) == 2)
            {
                std::cout << "cut tree!" << std::endl;
                world.at(tileUnderPlayerX).at(tileUnderPlayerY - world.at(0).size()) = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::DOWN)
        {
            int tileBelowPlayerPos = (tileUnderPlayerX + (tileUnderPlayerY + world.at(0).size()));
            bool isTileInBounds = tileBelowPlayerPos < (world.size() * world.at(0).size());

            if (isTileInBounds && world.at(tileUnderPlayerX).at(tileUnderPlayerY + world.at(0).size()) == 2)
            {
                std::cout << "cut tree!" << std::endl;
                world.at(tileUnderPlayerX).at(tileUnderPlayerY + world.at(0).size()) = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::LEFT)
        {
            int tileToLeftOfPlayerPos = (tileUnderPlayerX-1) + tileUnderPlayerY;
            bool isTileInBounds = tileToLeftOfPlayerPos >= 0;

            if (isTileInBounds && world.at(tileUnderPlayerX-1).at(tileUnderPlayerY) == 2)
            {
                std::cout << "cut tree!" << std::endl;
                world.at(tileUnderPlayerX-1).at(tileUnderPlayerY) = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::RIGHT)
        {
            int tileToRightOfPlayerPos = (tileUnderPlayerX+1) + tileUnderPlayerY;
            bool isTileInBounds = tileUnderPlayerX < world.at(0).size();

            if (isTileInBounds && world.at(tileUnderPlayerX+1).at(tileUnderPlayerY) == 2)
            {
                std::cout << "cut tree!" << std::endl;
                world.at(tileUnderPlayerX+1).at(tileUnderPlayerY) = 0;

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
