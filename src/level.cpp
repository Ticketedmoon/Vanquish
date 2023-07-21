#include "../include/level.h"
#include <cstdint>

Level::Level(Player* player)
{
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
    if (!map.load("./resources/assets/basic_tilemap.png", sf::Vector2u(32, 32), level, LEVEL_ROW_SIZE, TOTAL_ROWS_IN_LEVEL))
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

        int tmpTilePos = tileUnderPlayerX + (tileUnderPlayerY + LEVEL_ROW_SIZE);
        int tileBelowPlayerPos = tmpTilePos < (LEVEL_ROW_SIZE * TOTAL_ROWS_IN_LEVEL) ? tmpTilePos : (LEVEL_ROW_SIZE * TOTAL_ROWS_IN_LEVEL);

        if (level[tileBelowPlayerPos] > 0)
        {
            return;
        }

        player->updatePlayerPosition();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player->updateAnimation(clock, 3, PlayerDirection::UP);

        int tmpTilePos = tileUnderPlayerX + (tileUnderPlayerY - LEVEL_ROW_SIZE);
        int tileAbovePlayerPos = tmpTilePos < 0 ? 0 : tmpTilePos;

        if (level[tileAbovePlayerPos] > 0)
        {
            return;
        }

        player->updatePlayerPosition();
    }

}


void Level::checkForMoveHorizontalDirection(sf::Clock& clock, uint32_t tileUnderPlayerX, uint32_t tileUnderPlayerY)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player->updateAnimation(clock, 1, PlayerDirection::LEFT);

        int tmpTilePos = (tileUnderPlayerX-1) + tileUnderPlayerY;
        uint32_t tileToLeftOfPlayerPos = tmpTilePos < 0 ? 0 : tmpTilePos;
        if (level[tileToLeftOfPlayerPos] > 0)
        {
            return;
        }

        player->updatePlayerPosition();
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player->updateAnimation(clock, 2, PlayerDirection::RIGHT);

        int tmpTilePos = (tileUnderPlayerX + 1) + tileUnderPlayerY;
        uint32_t tileToRightOfPlayerPos = tmpTilePos > TOTAL_ROWS_IN_LEVEL ? TOTAL_ROWS_IN_LEVEL : tmpTilePos;
        if (level[tileToRightOfPlayerPos] > 0)
        {
            return;
        }

        player->updatePlayerPosition();
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
            int tileAbovePlayerPos = tileUnderPlayerX + (tileUnderPlayerY - LEVEL_ROW_SIZE);
            bool isTileInBounds = tileAbovePlayerPos >= 0;
            if (isTileInBounds && level[tileAbovePlayerPos] == 2)
            {
                std::cout << "cut tree!" << std::endl;
                level[tileAbovePlayerPos] = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::DOWN)
        {
            int tileBelowPlayerPos = (tileUnderPlayerX + (tileUnderPlayerY + LEVEL_ROW_SIZE));
            bool isTileInBounds = tileBelowPlayerPos < (TOTAL_ROWS_IN_LEVEL * LEVEL_ROW_SIZE);

            if (isTileInBounds && level[tileBelowPlayerPos] == 2)
            {
                std::cout << "cut tree!" << std::endl;
                level[tileBelowPlayerPos] = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::LEFT)
        {
            int tileToLeftOfPlayerPos = (tileUnderPlayerX-1) + tileUnderPlayerY;
            bool isTileInBounds = tileToLeftOfPlayerPos >= 0;

            if (isTileInBounds && level[tileToLeftOfPlayerPos] == 2)
            {
                std::cout << "cut tree!" << std::endl;
                level[tileToLeftOfPlayerPos] = 0;

                // TODO not ideal, fix me
                loadLevel();
            }
        }

        if (player->getPlayerDir() == PlayerDirection::RIGHT)
        {
            int tileToRightOfPlayerPos = (tileUnderPlayerX+1) + tileUnderPlayerY;
            bool isTileInBounds = tileUnderPlayerX < LEVEL_ROW_SIZE;

            if (isTileInBounds && level[tileToRightOfPlayerPos] == 2)
            {
                std::cout << "cut tree!" << std::endl;
                level[tileToRightOfPlayerPos] = 0;

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

uint32_t Level::getLevelWidth()
{
    return LEVEL_ROW_SIZE;
}

