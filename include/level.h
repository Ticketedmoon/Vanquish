#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <vector>
#include <json.hpp>
#include <fstream>
#include <optional>

#include "player.h"
#include "tilemap.h"
#include "enemy.h"

static constexpr size_t TOTAL_PLAYERS = 1;
static constexpr size_t TOTAL_ENEMIES = 8;
static constexpr size_t TOTAL_GAME_ENTITIES = TOTAL_PLAYERS + TOTAL_ENEMIES;

class Level : public GameComponent
{
    public:
        Level() = default;
        Level(std::shared_ptr<Player>& player, std::shared_ptr<TextureManager>& textureManager);

        void update(GameClock& gameClock) override;
        void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

        void interactWithNode(sf::Time deltaTime);
        void enableEntityTileHighlightsForDebug(std::unordered_map<EntityType, sf::Color> entityTypeTileColour);

        void initialiseGameEntities();
        uint32_t getWorldWidth();
        uint32_t getWorldHeight();

    private:
        void loadLevel();
        void checkForPlayerMovement(GameClock& gameClock, EntityDirection dir);

    private:
        TileMap map;
        std::vector<std::vector<uint32_t>> world;

        std::shared_ptr<Player> m_player;
        std::vector<std::shared_ptr<GameEntity>> gameEntities;
        std::shared_ptr<TextureManager> m_textureManager;
};
