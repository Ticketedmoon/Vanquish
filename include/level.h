#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <cstdint>
#include <vector>

#include "player.h"
#include "tilemap.h"

class Level
{
    public:
        Level(Player* player, uint32_t totalRows, uint32_t totalCols);

        void update(sf::Clock& clock);
        void chopTree();

        TileMap getTileMap();

    private:
        void loadLevel();
        void chopTreeForPlayerDirection(PlayerDirection dir, uint32_t tileX, uint32_t tileY);
        void checkForPlayerMovement(sf::Clock& clock, PlayerDirection dir, uint32_t spriteOffset);

        std::vector<std::vector<uint32_t>> world;
        Player* player;
        TileMap map;

        uint32_t spriteSheetX;
};
