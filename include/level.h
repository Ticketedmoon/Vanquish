#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <vector>
#include <json.hpp>
#include <fstream>

#include "player.h"
#include "tilemap.h"
#include "enemy.h"

class Level
{
    public:
        explicit Level(std::shared_ptr<Player>& player, std::vector<std::shared_ptr<GameEntity>>& gameEntities);

        void update(sf::Time& deltaTime, sf::Clock& worldClock);
        void interactWithNode(sf::Time& deltaTime);
        void debug(bool shouldClear);

        TileMap map;
        uint32_t getLevelWidth();
        uint32_t getLevelHeight();

    private:
        void loadLevel();
        void checkForPlayerMovement(sf::Time& deltaTime, sf::Clock& worldClock,
                                    EntityDirection dir, uint32_t spriteOffset);

        std::vector<std::vector<uint32_t>> world;
        std::shared_ptr<Player>& player;
        std::vector<std::shared_ptr<GameEntity>>& entities;
};
