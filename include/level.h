#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <vector>
#include <json.hpp>
#include <fstream>
#include <optional>

#include "player.h"
#include "tilemap.h"
#include "enemy.h"

class Level
{
    public:
        Level() = default;
        Level(std::shared_ptr<Player>& player, std::vector<std::shared_ptr<GameEntity>>& gameEntities);

        void update(sf::Time& deltaTime);
        void interactWithNode(sf::Time& deltaTime);
        void enableEntityTileHighlightsForDebug(std::unordered_map<EntityType, sf::Color> entityTypeTileColour);

        uint32_t getLevelWidth();
        uint32_t getLevelHeight();
        TileMap getMap();

    private:
        void loadLevel();
        void checkForPlayerMovement(sf::Time& deltaTime, EntityDirection dir);

        TileMap map;
        std::vector<std::vector<uint32_t>> world;
        std::shared_ptr<Player> player;
        std::vector<std::shared_ptr<GameEntity>> entities;
};
