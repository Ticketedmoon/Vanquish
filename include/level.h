#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <cstdint>
#include <vector>
#include <json.hpp>
#include <fstream>

#include "player.h"
#include "tilemap.h"

class Level
{
    public:
        Level(Player* player);

        void update(sf::Clock& clock);
        void interactWithNode();
        void debug();

        TileMap map;

    private:
        void loadLevel();
        void checkForPlayerMovement(sf::Clock& clock, PlayerDirection dir, uint32_t spriteOffset);

        std::vector<std::vector<uint32_t>> world;
        Player* player;

        uint32_t spriteSheetX;
};
