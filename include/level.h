#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <cstdint>
#include <vector>

#include "player.h"
#include "tilemap.h"

const uint32_t TOTAL_ROWS_IN_LEVEL = 10;
const uint32_t TOTAL_COLS_IN_LEVEL = 10;

class Level
{
    public:
        Level(Player* player);
        void update(sf::Clock& clock);
        void chopTree();
        TileMap getTileMap();

    private:
        void checkForMoveVerticalDirection(sf::Clock& clock, uint32_t tileUnderPlayerX, uint32_t tileUnderPlayerY);
        void checkForMoveHorizontalDirection(sf::Clock& clock, uint32_t tileUnderPlayerX, uint32_t tileUnderPlayerY);
        void loadLevel();
        void chopTreeForPlayerDirection(PlayerDirection dir, uint32_t tileX, uint32_t tileY);

        std::vector<std::vector<uint32_t>> world;
        Player* player;
        TileMap map;
        uint32_t spriteSheetX;
};
