#include "../include/level.h"

Level::Level(Player* player, uint32_t spriteSheetX)
{
    loadLevel();
    this->spriteSheetX = spriteSheetX;
    this->player = player;
}

void Level::update()
{
    checkForMoveVerticalDirection();
    checkForMoveHorizontalDirection();
    checkForChopTree();
}

void Level::loadLevel()
{
    if (!map.load("./resources/assets/basic_tilemap.png", sf::Vector2u(32, 32), level, LEVEL_ROW_SIZE, TOTAL_ROWS_IN_LEVEL))
    {
        std::cout << "Failed to load tileset" << std::endl;
        return;
    }
}

TileMap Level::getTileMap()
{
    return map;
}

void Level::checkForMoveVerticalDirection()
{
    sf::Vector2f playerPos = player->getPlayerPos();
    int tileUnderPlayerX = floor(playerPos.x/spriteSheetX);
    int tileUnderPlayerY = floor(playerPos.y/spriteSheetX) * LEVEL_ROW_SIZE;

    // TODO Move keyboard check to engine/controller class
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        player->updateAnimation(0, PlayerDirection::DOWN);

        if (tileUnderPlayerY >= ((TOTAL_ROWS_IN_LEVEL-1) * LEVEL_ROW_SIZE) || level[tileUnderPlayerX + (tileUnderPlayerY + LEVEL_ROW_SIZE)] > 0)
        {
            return;
        }

        player->updatePlayerPosition();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player->updateAnimation(3, PlayerDirection::UP);

        if (tileUnderPlayerY <= 0 || (level[tileUnderPlayerX + (tileUnderPlayerY - LEVEL_ROW_SIZE)] > 0))
        {
            return;
        }

        player->updatePlayerPosition();
    }

}


void Level::checkForMoveHorizontalDirection()
{
    sf::Vector2f playerPos = player->getPlayerPos();
    int tileUnderPlayerX = floor(playerPos.x/spriteSheetX);
    int tileUnderPlayerY = floor(playerPos.y/spriteSheetX) * LEVEL_ROW_SIZE;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player->updateAnimation(1, PlayerDirection::LEFT);

        if (tileUnderPlayerX == 0 || level[(tileUnderPlayerX - 1) + tileUnderPlayerY] > 0)
        {
            return;
        }

        player->updatePlayerPosition();
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player->updateAnimation(2, PlayerDirection::RIGHT);

        if (tileUnderPlayerX >= (LEVEL_ROW_SIZE-1) || level[(tileUnderPlayerX + 1) + tileUnderPlayerY] > 0)
        {
            return;
        }

        player->updatePlayerPosition();
    }
}

// TODO centralise common logic in this method
void Level::checkForChopTree()
{
    sf::Vector2f playerPos = player->getPlayerPos();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        int tileUnderPlayerX = floor(playerPos.x/spriteSheetX);
        int tileUnderPlayerY = floor(playerPos.y/spriteSheetX) * LEVEL_ROW_SIZE;

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
